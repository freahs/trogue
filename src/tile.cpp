#include "tile.hpp"

namespace trogue {
    std::vector<Tile> Tile::s_tiles({{" ", -1, -1}});

    Tile::Tile(const std::string symbol, int color, int bg_color)
    : symbol(symbol), color(color), bg_color(bg_color) { }

    Tile::Tile() : symbol(" "), color(-1), bg_color(-1) { }

    int Tile::create(const std::string symbol, int color, int bg_color) {
        s_tiles.push_back({symbol, color, bg_color});
        return (s_tiles.size() - 1);
    }

    const Tile& Tile::get(int id) {
        return s_tiles[id];
    }

    const Tile& Tile::blank() {
        return s_tiles[0];
    }
}
