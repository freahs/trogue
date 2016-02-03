#ifndef TROGUE_SCENE_H
#define TROGUE_SCENE_H

#include "format.hpp"
#include "tile.hpp"
#include "tilestack.hpp"

#include <ostream>
#include <vector>

namespace trogue {

    class Scene {
        private:
            size_t m_width;
            size_t m_height;

            std::vector<std::vector<TileStack>> m_tiles;
            std::vector<Tile>                   m_empty;

        public:
            Scene(size_t width, size_t height)
                : m_width(width), m_height(height), m_tiles(height, std::vector<TileStack>(width)) {
                    m_empty.push_back(Tile::empty);
                }

            void add(int x, int y, const Tile& tile) {
                m_tiles[y][x].add(tile);
            }

            void empty(Tile tile) {
                m_empty[0] = tile;
            }

            void emptyAlternative(Tile tile) {
                m_empty.push_back(tile);
            }

            const Tile& get(int x, int y) const {
                if(x < 0 || x >= static_cast<int>(m_width) || y < 0 || y >= static_cast<int>(m_height)) {
                    int alt_ratio = 7;
                    int empty = (y*width() + x) % (m_empty.size() + alt_ratio);
                    if (empty < alt_ratio) {
                        return m_empty[0];
                    } else {
                        return m_empty[empty - alt_ratio];
                    }
                } else {
                    return m_tiles[y][x].get();
                }
            }

            void printRow(std::ostream& os, const Format* prev_format, int y, int x1, int x2) const {
                if (y < 0 || y >= static_cast<int>(m_height)) {
                    for (int x = x1; x <= x2; ++x) {
                        Tile::empty.print(os, prev_format);
                    }
                    return;
                }

                for (int x = x1; x <= x2; ++x) {
                    if (x < 0 || x >= static_cast<int>(m_width)) {
                        Tile::empty.print(os, prev_format);
                    } else {
                        m_tiles[y][x].print(os, prev_format);
                    }
                }
            }

            size_t width() const { return m_width; }
            size_t height() const { return m_height; }
    };

}

#endif
