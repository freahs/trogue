#include "playersystem.hpp"

#include "tyra/tyra.hpp"

#include "components.hpp"

namespace trogue {

    PlayerSystem::PlayerSystem() {
        requireAll<PlayerComponent, PositionComponent, SightComponent, RenderComponent>();
    }

    void PlayerSystem::process(const tyra::System::Container& ids) {
        for (auto id : ids) {
            auto pc = world().component().get<PositionComponent>(id);
            world().component().get<RenderComponent>(id).update_pos(pc.y, pc.x);
        }
   

    }
}
