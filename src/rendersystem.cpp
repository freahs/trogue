#include "../inc/rendersystem.hpp"

#include "../inc/tyra/defs.hpp"
#include "../inc/tyra/system.hpp"

#include <unordered_set>

#include <ncursesw/ncurses.h>

namespace trogue {

    void EntitySystem::process(const std::unordered_set<tyra::EntityId>& ids) override {
        for (tyra::EntityId id : ids) {
            processEntity(id);
        }
    }        

    RenderSystem::RenderSystem() {
        requireAll<PositionComponent>();
        requireAll<DisplayComponent>();
    }

    void processEntity(tyra::EntityId id) {
        auto pos = world().component().get<PositionComponent>(id);
        auto disp = world().component().get<DisplayComponent>(id);
        

    }
}


