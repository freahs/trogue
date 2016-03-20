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

    bool Map::inRange(int y, int x) const {
        return y >= 0 && y < height() && x >= 0 && x < width();
    }

    void Map::floodFill() {
        typedef std::pair<int, int> point;
        std::deque<point> queue;
        std::vector<bool> visited(height()*width(), false);

        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {

                bool is_first = true;
                queue.push_back({j,i});

                while (queue.size() > 0) {
                    int y = queue.front().first;
                    int x = queue.front().second;
                    queue.pop_front();

                    if (visible(y, x) && !visited[y*width() + x]) {
                        visited[y*width() + x] = true;
                        is_first = false;
                        if (inRange(y-1, x-1)) { queue.push_back({y-1, x-1}); }
                        if (inRange(y-1, x))   { queue.push_back({y-1, x}); }
                        if (inRange(y-1, x+1)) { queue.push_back({y-1, x+1}); }
                        if (inRange(y,   x-1)) { queue.push_back({y,   x-1}); }
                        if (inRange(y,   x+1)) { queue.push_back({y,   x+1}); }
                        if (inRange(y+1, x-1)) { queue.push_back({y+1, x-1}); }
                        if (inRange(y+1, x))   { queue.push_back({y+1, x}); }
                        if (inRange(y+1, x-1)) { queue.push_back({y+1, x+1}); }
                    } else if (!visible(y, x) && !visited[y*width()] && !is_first) {
                        visited[y*width() + x] = true;
                        is_first = false;
                        visible(y, x, true);
                    }
                }
            }
        }

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

}
