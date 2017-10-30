#include "animationsystem.hpp"
#include "components.hpp"

namespace trogue {
    AnimationSystem::AnimationSystem() {
        requireAll<AnimationComponent>();
    }

    void AnimationSystem::process(const tyra::System::Container& ids) {
        for (auto id : ids) {
            world().component().get<AnimationComponent>(id).update(world().delta());
        }
    }
}
