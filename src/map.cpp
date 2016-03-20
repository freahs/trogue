#include "../inc/map.hpp"

#include <set>

namespace trogue {

    Map::Map(int height, int width)
        : m_height(height),
        m_width(width),
        m_entry_y(-1),
        m_entry_x(-1),
        m_tiles(m_height*m_width, false) {
        }

    bool Map::inRange(int y, int x) const {
        return y >= 0 && y < height() && x >= 0 && x < width();
    }

    void Map::floodFill(int start_y, int start_x) {
        typedef std::pair<int, int> point;

        std::set<point> queue;
        queue.insert({start_y, start_x});

        while (queue.size() > 0) {
            auto it = queue.begin();
            int y = it->first;
            int x = it->second;
            if (isWall(y,x)) {
                m_visible_tiles[y*width() + x] = true;
            } else if (m_visible_tiles[y*width() + x] == false) {
                m_visible_tiles[y*width() + x] = true;
                if (inRange(y-1, x-1)) { queue.insert({y-1, x-1}); }
                if (inRange(y-1, x))   { queue.insert({y-1, x}); }
                if (inRange(y-1, x+1)) { queue.insert({y-1, x+1}); }
                if (inRange(y,   x-1)) { queue.insert({y,   x-1}); }
                if (inRange(y,   x+1)) { queue.insert({y,   x+1}); }
                if (inRange(y+1, x-1)) { queue.insert({y+1, x-1}); }
                if (inRange(y+1, x))   { queue.insert({y+1, x}); }
                if (inRange(y+1, x-1)) { queue.insert({y+1, x+1}); }
            }
            queue.erase(it);
        }
    }

    void Map::finalize() {
    }

    void Map::set(int row, int col, int i) {
        m_tiles[row*width() + col] = i;
    }

    int Map::get(int row, int col) const {
        return m_tiles[row*width() + col];
    }

    int Map::height() const {
        return m_height;
    }

    int Map::width() const {
        return m_width;
    }

}
