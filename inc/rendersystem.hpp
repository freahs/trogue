#ifndef TROGUE_RENDERSYSTEM_H
#define TROGUE_RENDERSYSTEM_H

#include "tyra/defs.hpp"
#include "tyra/system.hpp"

#include <unordered_set>


namespace trogue {

    class EntitySystem : public tyra::System {
        protected:
            virtual void processEntity(tyra::EntityId) { }
            void process(const std::unordered_set<tyra::EntityId>& ids) override;
    };

    class RenderSystem : public EntitySystem {
        protected:
            void processEntity(tyra::EntityId id);

        public:
            RenderSystem();
    };

}

#endif
