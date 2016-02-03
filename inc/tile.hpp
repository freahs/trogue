#ifndef TROGUE_TILE_H
#define TROGUE_TILE_H

#include "format.hpp"

#include <ostream>
#include <string>


namespace trogue {

    class Tile : public Format {
        private:
            int         m_layer;
            std::string m_text;

        public:
            const static Tile empty;

            Tile()
                : Format(), m_text(" ") { }

            Tile(const std::string& text, int layer)
                : Format(), m_layer(layer), m_text(text) { }

            Tile(const Format& format, const std::string& text, int layer)
                : Format(format), m_layer(layer), m_text(text) { }

            Tile(const Tile& tile)
                : Format(tile), m_layer(tile.m_layer), m_text(tile.m_text) { }

            Tile& operator=(const Tile& other) {
                Format::operator=(other);
                this->m_layer = other.m_layer;
                this->m_text = other.m_text;
                return *this;
            }

            const std::string& str() const {
                return m_text;
            }

            int layer() const {
                return m_layer;
            }

            void print(std::ostream& os) const {
                apply(os);
                os << str();
            }

            void print(std::ostream& os, const Format* prev_format) const {
                apply(os, prev_format); 
                os << str();
            }
    }; 

    const Tile Tile::empty = Tile();
}

#endif
