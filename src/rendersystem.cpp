#include "../inc/rendersystem.hpp"

#include "../inc/tyra/tyra.hpp"
#include "../inc/components.hpp"
#include "../inc/display.hpp"

#include <unordered_set>

namespace trogue {

    void EntitySystem::process(const std::unordered_set<tyra::EntityId>& ids) {
        for (tyra::EntityId id : ids) {
            processEntity(id);
        }
    }

    RenderSystem::RenderSystem() : m_display(Display::instance()) {
        requireAll<PositionComponent>();
        requireAll<DisplayComponent>();
    }

    void RenderSystem::processEntity(tyra::EntityId id) {
        auto pos = world().component().get<PositionComponent>(id);
        auto disp = world().component().get<DisplayComponent>(id);
    }
}


