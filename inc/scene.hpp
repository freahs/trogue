#ifndef TROGUE_SCENE_H
#define TROGUE_SCENE_H

#include "tyra/tyra.hpp"

#include "map.hpp"
#include "shadowcast.hpp"
#include "entitystack.hpp"

#include <chrono>
#include <set>
#include <vector>

namespace trogue {

    // Usage:
    // When an entity is activated in the world, add it to the scene.
    // When an entity moves in the world, remove it and add it again
    // When an entity is inactivated in the world, remove it.
    //
    // Layers:
    // 0 Floor and walls
    // 1 Items
    // 2 Characters
    // 3 Effects

    static const int    s_max_visibility_range = 20;
    static const int    s_tile_rotation_time = 1000; // in milliseconds

    class Scene {
    private:
        int                 m_center_y;
        int                 m_center_x;
        int                 m_elapsed_time;
        Map<EntityStack>    m_stacks;
        Shadowcast          m_shadowcast;

    public:
        Scene(int height, int width);

        bool opaque(int row, int col) const;
        void opaque(int row, int col, bool opaque);
        bool visible(int row, int col) const;
        bool visited(int row, int col) const;

        void update(int y, int x, int range, int delta);
        void add(int row, int col, int layer, tyra::EntityId id);
        void remove(int row, int col, tyra::EntityId id);
        tyra::EntityId get(int row, int col) const;

        const EntityStack& all(int row, int col) const;

        int width() const;
        int height() const;

        int centerY() const;
        int centerX() const;

        bool inRange(int y, int x) const;

        void print() const;
    };

}

#endif
