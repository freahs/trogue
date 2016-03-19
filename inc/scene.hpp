#ifndef TROGUE_SCENE_H
#define TROGUE_SCENE_H

#include "tyra/tyra.hpp"

#include <array>
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

    class Scene {
    private:
        class EntityStack {
        private:
            std::set<tyra::EntityId>                    m_entities;
            std::set<tyra::EntityId>::iterator    m_current;

        public:
            EntityStack();
            void update();
            bool add(tyra::EntityId);
            bool remove(tyra::EntityId);
            tyra::EntityId get() const;
        };

        typedef std::array<EntityStack, num_layers> LayerStack;

        size_t                      m_height;
        size_t                      m_width;
        std::vector<LayerStack>     m_stacks;

        const EntityStack& getStack(int row, int col, int layer) const;
        EntityStack& getStack(int row, int col, int layer);

    public:
        Scene(int height, int width);

        void update();
        void add(int row, int col, size_t layer, tyra::EntityId id);
        void remove(int row, int col, size_t layer, tyra::EntityId id);
        tyra::EntityId get(int row, int col, size_t layer) const;

        size_t width() const  { return m_width; }
        size_t height() const { return m_height; }
    };

}

#endif
