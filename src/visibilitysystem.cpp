#include "visibilitysystem.hpp"

#include "tyra/tyra.hpp"

#include "components.hpp"
#include "map.hpp"

namespace trogue {

    VisibilitySystem::VisibilitySystem(Scene& scene) : m_scene(&scene) {
        requireAll<PositionComponent, TileComponent>();
    }

    Scene& VisibilitySystem::scene() {
        return *m_scene;
    }


    void VisibilitySystem::process(const tyra::System::Container& ids) {
        auto pid = world().tag("PLAYER");
        auto player_pos = world().component().get<PositionComponent>(pid);
        auto player_sight = world().component().get<SightComponent>(pid);
        scene().update(player_pos.y, player_pos.x, player_sight.range, world().delta());

        for (auto id : ids) {
            auto pos = world().component().get<PositionComponent>(id);
            auto tc = world().component().get<TileComponent>(id);

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
            if(scene().visible(pos.y, pos.x)) {
                if (sc != nullptr) {
                    scene().remove(sc->y, sc->x, id);
                    world().component().remove<ShadowComponent>(id);
                }
                if (vc == nullptr) {
                    world().component().add<VisibleComponent>(id);
                    scene().add(pos.y, pos.x, tc.layer, id);
                }

                // If it's not visible, remove its visibility component. It should keep
                // its render component UNLESS the position of the render component is
                // visible.
            } else {
                if (vc != nullptr) {
                    world().component().remove<VisibleComponent>(id);
                    world().component().add<ShadowComponent>(id, pos.y, pos.x);
                }
                if (sc != nullptr && scene().visible(sc->y, sc->x)) {
                    scene().remove(sc->y, sc->x, id);
                    world().component().remove<ShadowComponent>(id);
                }
            }
        }
    }
}
