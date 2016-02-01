#ifndef TROGUE_RENDERSYSTEM_H
#define TROGUE_RENDERSYSTEM_H

#include "tyra/system.hpp"
#include "display.hpp"

#include <unordered_set>


namespace trogue {


    class EntitySystem : public tyra::System {
        protected:
            virtual void processEntity(tyra::EntityId) { }
            void process(const std::unordered_set<tyra::EntityId>& ids) override;
    };

    class RenderSystem : public EntitySystem {
        private:
            Display*        m_display;
            SceneManager*   m_scene_manager;

            Display& display();
            SceneManager& scene();

        protected:
            void processEntity(tyra::EntityId id);

        public:
            RenderSystem();
    };

}

#endif
