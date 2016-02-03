#ifndef TROGUE_TILESTACK_H
#define TROGUE_TILESTACK_H

#include "format.hpp"
#include "tile.hpp"

#include <algorithm>
#include <vector>

namespace trogue {

    class TileStack {
        private:
            std::vector<Tile>   m_tiles;
            Tile                m_current_tile;

        public:
            TileStack()
                : m_tiles(0), m_current_tile(Tile::empty) {
                }

            void add(const Tile& tile) {
                m_tiles.push_back(tile);

                std::sort(m_tiles.begin(), m_tiles.end(), [](const Tile& a, const Tile& b)->bool {
                        return a.layer() < b.layer();
                        });

                m_current_tile = m_tiles[0];

                for (size_t i = 1; i < m_tiles.size() && m_current_tile.bgcolor() == -1; ++i) { 
                    m_current_tile.bgcolor(m_tiles[i].bgcolor());
                }
            }

            const Tile& get() const {
                return m_current_tile;
            }

            void print(std::ostream& os, const Format* format) const {
                m_current_tile.print(os, format);
            }
    };

}

#endif
