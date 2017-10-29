#include "playersystem.hpp"

#include "tyra/tyra.hpp"

#include "components.hpp"

namespace trogue {

    PlayerSystem::PlayerSystem(Scene& scene) : m_scene(&scene) {
        requireAll<PlayerComponent>();
        requireAll<PositionComponent>();
        requireAll<SightComponent>();
    }

    Scene& PlayerSystem::scene() {
        return *m_scene;
    }

    void PlayerSystem::process(const tyra::System::Container& ids) {
        for (auto id : ids) {
            auto pos = world().component().get<PositionComponent>(id);
            auto sight = world().component().get<SightComponent>(id);
        }
    }
}
