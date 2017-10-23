#ifndef TROGUE_COMPONENTS_H
#define TROGUE_COMPONENTS_H

#include "tyra/componentmanager.hpp"

#include <string>

namespace trogue {

    struct PlayerComponent : public tyra::Component { };

    struct AttributeComponent : public tyra::Component {
        bool translucent = true;
        bool solid = false;
        AttributeComponent(bool translucent, bool solid)
        : translucent(translucent), solid(solid) { }
    };

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

    struct VisibleComponent : public tyra::Component { };

    struct RenderComponent : public tyra::Component {
        int y, x;
        RenderComponent(int y, int x) : y(y), x(x) { }
        void update(int new_y, int new_x) { y = new_y; x = new_x; }
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
