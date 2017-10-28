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
        //typedef std::chrono::high_resolution_clock      Time;
        //typedef std::chrono::milliseconds               Ms;
        //typedef std::chrono::system_clock::time_point   TimePoint;

        // EntityStack keeps track of the content of a tile which id divided into layers.
        // If there are multiple entities occupying the same tile, these are rotated with
        // fixed intervals, which is handeled by the Scene.
        /*
        class EntityStack {
        private:
            std::set<tyra::EntityId>                    m_entities;
            std::set<tyra::EntityId>::const_iterator    m_current;

        public:
            EntityStack();
            void update();
            bool add(tyra::EntityId);
            bool remove(tyra::EntityId);
            tyra::EntityId get() const;
        };
        */

        int                         m_num_layers;
        int                         m_center_y;
        int                         m_center_x;
        //int                         m_delta;
        //TimePoint                   m_last_update;
        int                         m_elapsed_time;
        std::vector<EntityStack>    m_stacks;
        Map<bool>                   m_terrain_map;
        Shadowcast                  m_shadowcast;

        const EntityStack& getStack(int row, int col, int layer) const;
        EntityStack& getStack(int row, int col, int layer);

    public:
        Scene(Map<bool> map, int num_layers);

        bool visible(int row, int col) const;
        bool visited(int row, int col) const;
        bool blocked(int row, int col) const;

        void update(int y, int x, int range, int delta);
        void add(int row, int col, int layer, tyra::EntityId id);
        void remove(int row, int col, int layer, tyra::EntityId id);
        tyra::EntityId get(int row, int col, int layer) const;

        int width() const;
        int height() const;
        int layers() const;

        int centerY() const;
        int centerX() const;

        bool inRange(int y, int x) const;

        void print() const;
    };

}

#endif
