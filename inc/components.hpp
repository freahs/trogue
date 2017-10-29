#ifndef TROGUE_COMPONENTS_H
#define TROGUE_COMPONENTS_H

#include "tyra/componentmanager.hpp"

#include "tile.hpp"
#include <set>

#include <string>

namespace trogue {

    struct PlayerComponent : public tyra::Component { };

    struct AIComponent : public tyra::Component {
        int speed;
        int elapsed;
        AIComponent(int speed) : speed(speed), elapsed(0) { }

        bool update(int delta) {
            elapsed += delta;
            if (elapsed >= speed) {
                elapsed = (elapsed % speed);
                return true;
            }
            return false;
        }
    };

    struct CollisionComponent : public tyra::Component {
        std::vector<tyra::EntityId> targets;
        void add(tyra::EntityId id) { targets.push_back(id); }
    };


    enum class Attribute {OPAQUE, SOLID};

    struct AttributeComponent : public tyra::Component {
        std::set<Attribute> attributes;
        AttributeComponent() = default;
        AttributeComponent(std::initializer_list<Attribute> attribs) : attributes(attribs) { }
        void add(Attribute attribute) { attributes.insert(attribute); }
        bool has(Attribute attribute) { return attributes.find(attribute) != attributes.end(); }
    };

    struct SightComponent : public tyra::Component {
        int range;
        SightComponent(int range) : range(range) { }
    };

    struct PositionComponent : public tyra::Component {
        int y, x;
        PositionComponent(int y, int x) : y(y), x(x) { }
    };

    struct ShadowComponent : public tyra::Component {
        int y, x;
        ShadowComponent(int y, int x) : y(y), x(x) { }
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

    class TileComponent : public tyra::Component {
    private:
        int id_a;
        int id_b;
    public:
        int layer;
        TileComponent(int id, int blocked_id, int layer) : id_a(id), id_b(blocked_id), layer(layer) { }
        const Tile& normal() const { return Tile::get(id_a); }
        const Tile& blocked() const { return Tile::get(id_b); }
    };

}


#endif
