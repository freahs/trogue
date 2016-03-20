#include "../inc/map.hpp"

#include <deque>
#include <set>

namespace trogue {

    Map::Map(int height, int width)
        : m_height(height),
        m_width(width),
        m_tiles(m_height*m_width, 0),
        m_visible(m_height*m_width, false),
        m_walkable(m_height*m_width, false) {
        }

    void Map::tile(int y, int x, int tile) {
        m_tiles[y*width() + x] = tile;
    }

    void Map::visible(int y, int x, bool visible) {
        m_visible[y*width() + x] = visible;
    }

    void Map::walkable(int y, int x, bool walkable) {
        m_walkable[y*width() + x] = walkable;
    }

    int Map::tile(int y, int x) const {
        return m_tiles[y*width() + x];
    }

    bool Map::visible(int y, int x) const {
        return m_visible[y * width() + x];
    }

    bool Map::walkable(int y, int x) const {
        return m_walkable[y * width() + x];
    }

    int Map::height() const {
        return m_height;
    }

    int Map::width() const {
        return m_width;
    }


    MapBuilder::MapBuilder(int height, int width)
        : m_map(height, width) { }

    void MapBuilder::floodFill() {
        typedef std::pair<int, int> point;
        std::deque<point> queue;
        std::vector<bool> visited(map().height()*map().width(), false);

        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {

                bool is_first = true;
                queue.push_back({j,i});

                while (queue.size() > 0) {
                    int y = queue.front().first;
                    int x = queue.front().second;
                    queue.pop_front();

                    if (map().visible(y, x) && !visited[y*map().width() + x]) {
                        visited[y*map().width() + x] = true;
                        is_first = false;
                        if (inRange(y-1, x-1)) { queue.push_back({y-1, x-1}); }
                        if (inRange(y-1, x))   { queue.push_back({y-1, x}); }
                        if (inRange(y-1, x+1)) { queue.push_back({y-1, x+1}); }
                        if (inRange(y,   x-1)) { queue.push_back({y,   x-1}); }
                        if (inRange(y,   x+1)) { queue.push_back({y,   x+1}); }
                        if (inRange(y+1, x-1)) { queue.push_back({y+1, x-1}); }
                        if (inRange(y+1, x))   { queue.push_back({y+1, x}); }
                        if (inRange(y+1, x-1)) { queue.push_back({y+1, x+1}); }
                    } else if (!map().visible(y, x) &&
                               !visited[y*map().width()] && !is_first) {
                        visited[y*map().width() + x] = true;
                        is_first = false;
                        map().visible(y, x, true);
                    }
                }
            }
        }
    }

    bool MapBuilder::inRange(int y, int x) const {
        return y >= 0 && y < map().height() && x >= 0 && x < map().width();
    }

    Map& MapBuilder::map() {
        return m_map;
    }

    const Map& MapBuilder::map() const {
        return m_map;
    }


    const Map& MapBuilder::finalize() {
        floodFill();
        return m_map;
    }

    AutomataMapBuilder::AutomataMapBuilder(int height, int width, float init_wall_prob)
        : MapBuilder(height, width),
        m_walls(map().height()*map().width(), true) {
            std::default_random_engine rng(std::random_device{}());
            std::uniform_real_distribution<float> dist(0.0, 1.0);

            for(int y = 1; y < map().height() - 1; ++y) {
                for(int x = 1; x < map().width() - 1; ++x) {
                    m_walls[y*map().width() + x] = dist(rng) < init_wall_prob; 
                }
            }
        }

    bool AutomataMapBuilder::isWall(int y, int x) const {
        return m_walls[y*map().width() + x];
    }

    void AutomataMapBuilder::isWall(int y, int x, bool val) {
        m_walls[y*map().width() + x] = val;
    } 


    void AutomataMapBuilder::build(int r1_limit, int r2_limit) {
        std::vector<bool> tmp_walls(m_walls.size(), true);

        for (int y = 1; y < map().height() - 1; ++y) {
            for (int x = 1; x < map().width() - 1; ++x) {

                int r1_adj = 0;
                int r2_adj = 0;
                for(int rel_y = -2; rel_y <= 2; ++rel_y) {
                    for (int rel_x = -2; rel_x <= 2; ++rel_x) {
                        int adj_y = y + rel_y;
                        int adj_x = x + rel_x;
                        if (!inRange(adj_y, adj_x) || isWall(adj_y, adj_x)) {
                            if (std::abs(rel_y) < 2 && std::abs(rel_x) < 2) {
                                ++r1_adj;
                            }
                            ++r2_adj;
                        }
                    }
                }

                if(r1_adj >= r1_limit || r2_adj <= r2_limit) {
                    tmp_walls[y*map().width() + x] = true;
                } else {
                    tmp_walls[y*map().width() + x] = false;
                }
            }
        }
        std::cout << std::endl;
        m_walls = tmp_walls;
    }

    const Map& AutomataMapBuilder::finalize() {
        for (int y = 0; y < map().height(); ++y) {
            for (int x = 0; x <map().width(); ++x) {
                map().visible(y, x, !isWall(y, x));
            }
        }
        return MapBuilder::finalize();
    }

    void AutomataMapBuilder::print() {
        for (int y = 0; y < map().height(); ++y) {
            for (int x = 0; x <map().width(); ++x) {
                std::cout << isWall(y, x);
            }
            std::cout << std::endl;
        }
    }

}
