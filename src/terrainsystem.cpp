#include "terrainsystem.hpp"

#include "tyra/tyra.hpp"

#include "shadowcast.hpp"

namespace trogue {
    void TerrainSystem::entityAdded(tyra::EntityId id) {
        if (world().component().get<AttributeComponent>(id).has(Attribute::OPAQUE)) {
            auto pos = world().component().get<PositionComponent>(id);
            world().scene().opaque(pos.y, pos.x, true);
        }
    }

    void TerrainSystem::entityRemoved(tyra::EntityId id) {
        if (world().component().get<AttributeComponent>(id).has(Attribute::OPAQUE)) {
            auto pos = world().component().get<PositionComponent>(id);
            world().scene().opaque(pos.y, pos.x, true);
        }
    }

    TerrainSystem::TerrainSystem() {
        requireAll<AttributeComponent>();
    }
}
