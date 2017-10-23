#include "aisystem.hpp"

#include "tyra/tyra.hpp"

#include "components.hpp"

#include <cstdlib>

namespace trogue {

    AISystem::AISystem() {
        requireAll<AIComponent, PositionComponent>();
    }

    void AISystem::process(const tyra::System::Container& ids) {
        for (auto id : ids) {
            auto ai = &world().component().get<AIComponent>(id);
            auto pos = world().component().get<PositionComponent>(id);
            auto sight = world().component().get<SightComponent>(id);
            if (ai->update(world().delta())) {
                int vy = (rand() % 3) - 1;
                int vx = (rand() % 3) - 1;
                world().component().add<MovementComponent>(id, vy, vx);
            }
        }
    }
}
