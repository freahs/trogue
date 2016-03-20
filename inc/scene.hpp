#ifndef TROGUE_SCENE_H
#define TROGUE_SCENE_H

#include "tyra/tyra.hpp"

#include "map.hpp"
#include "shadowcast.hpp"

#include <array>
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

    static const size_t num_layers = 4;
    static const int    max_visibility_range = 20;
    static const int    tile_rotation_time = 1000;

    class Scene {
    private:
        typedef std::chrono::high_resolution_clock      Time;
        typedef std::chrono::milliseconds               Ms;
        typedef std::chrono::system_clock::time_point   TimePoint;

        class EntityStack {
        private:
            std::set<tyra::EntityId>            m_entities;
            std::set<tyra::EntityId>::iterator  m_current;

        public:
            EntityStack();
            void update();
            bool add(tyra::EntityId);
            bool remove(tyra::EntityId);
            tyra::EntityId get() const;
        };

        typedef std::array<EntityStack, num_layers> LayerStack;

        int                         m_center_y;
        int                         m_center_x;
        int                         m_delta;
        TimePoint                   m_last_update;
        std::vector<LayerStack>     m_stacks;
        mutable std::vector<bool>   m_visited;
        ShadowCast                  m_shadowcast;
        Map                         m_map;

        const EntityStack& getStack(int row, int col, int layer) const;
        EntityStack& getStack(int row, int col, int layer);

    public:
        Scene(Map map);

        bool visible(int row, int col) const;
        bool visited(int row, int col) const;

        void update(int y, int x);
        void add(int row, int col, size_t layer, tyra::EntityId id);
        void remove(int row, int col, size_t layer, tyra::EntityId id);
        tyra::EntityId get(int row, int col, size_t layer) const;

        int width() const; 
        int height() const;

        void print() const;
    };

}

#endif
