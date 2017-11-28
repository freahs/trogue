#ifndef TROGUE_TILE_H
#define TROGUE_TILE_H

#include <string>
#include <ostream>
#include <memory>
#include <vector>

namespace trogue {

    class Tile {
    private:
        static std::vector<Tile> s_tiles;


    public:
        std::string symbol;
        int         color;
        int         bg_color;

        Tile();
        Tile(const std::string symbol, int color, int bg_color);

        static int create(const std::string symbol, int color, int bg_color);
        static const Tile& blank();
        static const Tile& get(int id);

        /*

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
            if (this->symbol != other.symbol) { return false; }
            return true;
        }

        bool operator<(const Tile& other) {
            if (this->color != other.color) {
                return this->color < other.color;
            }
            if (this->bg_color != other.bg_color) {
                return this->bg_color < other.bg_color;
            }
            return this->symbol < other.symbol;
        }
        */

    };

}

#endif
