#ifndef TROGUE_COMPONENTS_H
#define TROGUE_COMPONENTS_H

#include "tyra/componentmanager.hpp"

#include <string>

namespace trogue {

    struct PlayerComponent : public tyra::Component { };

    struct SightComponent : public tyra::Component {
        int range;
        SightComponent(int range) : range(range) { }
    };

    struct PositionComponent : public tyra::Component {
        int y, x;
        PositionComponent(int y, int x) : y(y), x(x) { }
    };

    struct MovementComponent : public tyra::Component {
        int vy, vx;
        MovementComponent(int vy, int vx) : vy(vy), vx(vx) { } 
    };

    struct TileComponent : public tyra::Component {
        std::string     symbol;
        int             color;
        int             blocked_color;
        int             bg_color;
        int             blocked_bg_color;
        int             layer;

        TileComponent(std::string symbol,
                      int color, int blocked_color,
                      int bg_color, int blocked_bg_color, int layer)
            : symbol(symbol),
            color(color), blocked_color(blocked_color),
            bg_color(bg_color), blocked_bg_color(blocked_bg_color),
            layer(layer) {
            } 
    };

}


#endif
