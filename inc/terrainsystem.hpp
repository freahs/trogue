#ifndef TROGUE_TERRAINSYSTEM_H
#define TROGUE_TERRAINSYSTEM_H

#include "tyra/tyra.hpp"

#include "ecs.hpp"

#include "shadowcast.hpp"

#include "components.hpp"


namespace trogue {
    class TerrainSystem : public TSystem {
    private:
        void entityAdded(tyra::EntityId id) override;
        void entityRemoved(tyra::EntityId id) override;

    public:
        TerrainSystem();
    };
}

#endif
