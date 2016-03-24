#include "movementsystem.hpp"

#include "components.hpp"

#include "tyra/tyra.hpp"

#include <unordered_set>


namespace trogue {

    MovementSystem::MovementSystem(Scene& scene) : m_scene(&scene) {
        requireAll<TileComponent>();
        requireAll<PositionComponent>();
        requireAll<MovementComponent>();
    }

    Scene& MovementSystem::scene() {
        return *m_scene;
    }

    void MovementSystem::process(const tyra::System::Container& ids) {

        for (tyra::EntityId id : ids) {
            auto tile = &world().component().get<TileComponent>(id);
            auto position = &world().component().get<PositionComponent>(id);
            auto movement = &world().component().get<MovementComponent>(id);
            scene().remove(position->y, position->x, tile->layer, id);
            if (scene().inRange(position->y + movement->vy, position->x)) {
                position->y += movement->vy;
            }

            if (scene().inRange(position->y, position->x + movement->vx)) {
                position->x += movement->vx;
            }
            scene().add(position->y, position->x, tile->layer, id);
            world().component().remove<MovementComponent>(id);
        }

        /*
        auto player_position = world().component().get<PositionComponent>(world().tag("PLAYER"));
        int py = player_position.y;
        int px = player_position.x;

        scene().update(py, px, 20);
        */
    }

}
