#include "movementsystem.hpp"

#include "components.hpp"

#include "tyra/tyra.hpp"

#include <unordered_set>


namespace trogue {

    MovementSystem::MovementSystem(Scene& scene) : m_scene(&scene) {
        requireAll<TileComponent, PositionComponent, MovementComponent>();
    }

    Scene& MovementSystem::scene() {
        return *m_scene;
    }

    void MovementSystem::process(const tyra::System::Container& ids) {

        for (tyra::EntityId id : ids) {
            auto tile = &world().component().get<TileComponent>(id);
            auto position = &world().component().get<PositionComponent>(id);
            auto movement = &world().component().get<MovementComponent>(id);

            auto new_y = position->y + movement->vy;
            auto new_x = position->x + movement->vx;

            auto visible = world().component().valid<VisibleComponent>(id);
            if (visible) {
                scene().remove(position->y, position->x, id);
            }
            if (scene().inRange(new_y, position->x) && !scene().blocked(new_y, position->x)) {
                position->y += movement->vy;
            }

            if (scene().inRange(position->y, new_x) && !scene().blocked(position->y, new_x)) {
                position->x += movement->vx;
            }
            if (visible) {
                scene().add(position->y, position->x, tile->layer, id);
            }
            world().component().remove<MovementComponent>(id);
        }

    }

}
