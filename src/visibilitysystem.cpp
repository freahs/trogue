#include "visibilitysystem.hpp"

#include "tyra/tyra.hpp"

#include "components.hpp"
#include "map.hpp"

namespace trogue {

    VisibilitySystem::VisibilitySystem(int height, int width) : m_height(height), m_width(width) {
        requireAll<PositionComponent, AttributeComponent, TileComponent>();
        exclude<PlayerComponent>();
    }


    void VisibilitySystem::process(const tyra::System::Container& ids) {
        auto m = Map<bool>(m_height, m_width);
        for (auto id : ids) {
            auto pos = world().component().get<PositionComponent>(id);
            auto attr = world().component().get<AttributeComponent>(id);
            if (attr.translucent == false) {
                m[pos.y][pos.x] = true;
            }
        }

        auto pid = world().tag("PLAYER");
        auto player_pos = world().component().get<PositionComponent>(pid);
        auto player_sight = world().component().get<SightComponent>(pid);
        Shadowcast sc(m);
        sc.update(player_pos.y, player_pos.x, player_sight.range);

        for (auto id : ids) {
            auto pos = world().component().get<PositionComponent>(id);
            auto tc = world().component().get<TileComponent>(id);

            RenderComponent* rc = nullptr;
            if (world().component().valid<RenderComponent>(id)) {
                rc = &world().component().get<RenderComponent>(id);
            }

            VisibleComponent* vc = nullptr;
            if (world().component().valid<VisibleComponent>(id)) {
                vc = &world().component().get<VisibleComponent>(id);
            }

            // If the entitys position is visible, add a visibility component
            // and add a render component or update an existing one.
            if(sc.visible(pos.y, pos.x)) {
                if (vc == nullptr) {
                    world().component().add<VisibleComponent>(id);
                }
                if (rc == nullptr) {
                    world().component().add<RenderComponent>(id, pos.y, pos.x, tc.symbol, tc.color, tc.bg_color, tc.layer);
                } else {
                    rc->update_pos(pos.y, pos.x);
                    rc->update_tile(tc.color, tc.bg_color);
                }

            // If it's not visible, remove its visibility component. It should keep
            // its render component UNLESS the position of the render component is
            // visible.
            } else {
                if (vc != nullptr) {
                    world().component().remove<VisibleComponent>(id);
                    if (rc != nullptr) {
                        rc->update_tile(tc.blocked_color, tc.blocked_bg_color);
                    }
                }
                if (rc != nullptr && sc.visible(rc->y, rc->x)) {
                    world().component().remove<RenderComponent>(id);
                }
            }
        }
    }
}
