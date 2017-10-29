#include "playersystem.hpp"

#include "tyra/tyra.hpp"

#include "components.hpp"

namespace trogue {

    PlayerSystem::PlayerSystem() {
        requireAll<PlayerComponent, PositionComponent, SightComponent>();
    }

    void PlayerSystem::process(const tyra::System::Container& ids) {
        for (auto id : ids) {
            auto pos = world().component().get<PositionComponent>(id);
            auto sight = world().component().get<SightComponent>(id);
        }
    }
}
