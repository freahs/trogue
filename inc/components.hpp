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

    struct TileComponent : public tyra::Component {
        int layer;
        TileComponent(int layer) : layer(layer) { }
        virtual const Tile& normal() const = 0;
        virtual const Tile& blocked() const = 0;
    };

    class SharedTileComponent : public TileComponent {
    private:
        int id_n;
        int id_b;
    public:
        SharedTileComponent(int id_n, int id_b, int layer)
        : TileComponent(layer), id_n(id_n), id_b(id_b){ }
        const Tile& normal() const override { return Tile::get(id_n); }
        const Tile& blocked() const override { return Tile::get(id_b); }
    };

    class CustomTileComponent : public TileComponent {
    private:
        const Tile tile_n;
        const Tile tile_b;
    public:
        CustomTileComponent(Tile&& tile_n, Tile&& tile_b, int layer)
        : TileComponent(layer), tile_n(tile_n), tile_b(tile_b) { }
    };

}


#endif
