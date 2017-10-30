#include "visibilitysystem.hpp"

#include "tyra/tyra.hpp"

#include "components.hpp"
#include "map.hpp"
#include "shadowcast.hpp"

namespace trogue {

    VisibilitySystem::VisibilitySystem() {
        requireAll<PositionComponent, TileComponent>();
    }

    void VisibilitySystem::process(const tyra::System::Container& ids) {
        auto pid = world().tag("PLAYER");
        auto player_pos = world().component().get<PositionComponent>(pid);
        auto player_sight = world().component().get<SightComponent>(pid);
        Shadowcast s_cast(world().scene().height(), world().scene().width());
        for (auto id : ids) {
            if (world().component().valid<AttributeComponent>(id)) {
                if (world().component().get<AttributeComponent>(id).has(Attribute::OPAQUE)) {
                    auto pos = world().component().get<PositionComponent>(id);
                    s_cast.opaque(pos.y, pos.x, true);
                }
            }
        }
        world().scene().update(world().delta());
        s_cast.update(player_pos.y, player_pos.x, player_sight.range);
        s_cast.visible(player_pos.y, player_pos.x, true);

        for (auto id : ids) {
            auto pos = world().component().get<PositionComponent>(id);
            auto tc = &world().component().get<TileComponent>(id);

            VisibleComponent* vc = nullptr;
            if (world().component().valid<VisibleComponent>(id)) {
                vc = &world().component().get<VisibleComponent>(id);
            }

            ShadowComponent* sc = nullptr;
            if (world().component().valid<ShadowComponent>(id)) {
                sc = &world().component().get<ShadowComponent>(id);
            }

            // If the entitys position is visible, add a visibility component
            // and add a render component or update and existing one.
            if(s_cast.visible(pos.y, pos.x)) {
                if (sc != nullptr) {
                    world().scene().remove(sc->y, sc->x, id);
                    world().component().remove<ShadowComponent>(id);
                }
                if (vc == nullptr) {
                    world().component().add<VisibleComponent>(id);
                    world().scene().add(pos.y, pos.x, tc->layer, id);
                }

                // If it's not visible, remove its visibility component. It should keep
                // its render component UNLESS the position of the render component is
                // visible.
            } else {
                if (vc != nullptr) {
                    world().component().remove<VisibleComponent>(id);
                    world().component().add<ShadowComponent>(id, pos.y, pos.x);
                }
                if (sc != nullptr && s_cast.visible(sc->y, sc->x)) {
                    world().scene().remove(sc->y, sc->x, id);
                    world().component().remove<ShadowComponent>(id);
                }
            }
        }



    }
}
