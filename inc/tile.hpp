#ifndef TROGUE_TILE_H
#define TROGUE_TILE_H

#include <string>
#include <ostream>

namespace trogue {

    struct Tile {
        std::string symbol   = " ";
        int         color    = -1;
        int         bg_color = -1;
        int         layer    = -1;

        Tile() = default;

        Tile(const std::string symbol, int color, int bg_color, int layer)
        : symbol(symbol), color(color), bg_color(bg_color), layer(layer) { }

        bool complete() const {
            if (color == -1) { return false; }
            if (bg_color == -1) { return false; }
            return true;
        }

        void merge(const Tile& other) {
            if (color == -1) { color = other.color; }
            if (bg_color == -1) { bg_color = other.bg_color; }
        }

        bool operator==(const Tile& other) {
            if (this->color != other.color) { return false; }
            if (this->bg_color != other.bg_color) { return false; }
            if (this->layer != other.layer) { return false; }
            if (this->symbol != other.symbol) { return false; }
            return true;
        }

        bool operator<(const Tile& other) {
            if (this->layer != other.layer) {
                return this->layer > other.layer;
            }
            if (this->color != other.color) {
                return this->color < other.color;
            }
            if (this->bg_color != other.bg_color) {
                return this->bg_color < other.bg_color;
            }
            return this->symbol < other.symbol;
        }

    };
}

#endif
