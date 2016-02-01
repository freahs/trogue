#ifndef TROGUE_COMPONENTS_H
#define TROGUE_COMPONENTS_H

#include "tyra/componentmanager.hpp"

#include <string>

namespace trogue {

    struct PositionComponent {
        int x, y;
        PositionComponent(int x, int y) : x(x), y(y) { }
    };

    struct DisplayComponent {
        std::string     symbol;
        int             color;
        int             bg_color;
        int             layer;

        DisplayComponent(std::string symbol, int color, int bg_color, int layer)
        : symbol(symbol), color(color), bg_color(bg_color), layer(layer) {
        } 
    };
}

#endif
