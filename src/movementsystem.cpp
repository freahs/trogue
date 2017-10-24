#include "movementsystem.hpp"

#include "components.hpp"

#include "tyra/tyra.hpp"

#include <unordered_set>


namespace trogue {

    MovementSystem::MovementSystem(int height, int width) : m_height(height), m_width(width) {
        requireAll<TileComponent, PositionComponent, MovementComponent>();
    }


    void MovementSystem::process(const tyra::System::Container& ids) {

        for (tyra::EntityId id : ids) {
            auto position = &world().component().get<PositionComponent>(id);
            auto movement = &world().component().get<MovementComponent>(id);

            auto new_y = position->y + movement->vy;
            if (new_y >= 0 && new_y < m_height) {
                position->y += movement->vy;
            }

            auto new_x = position->x + movement->vx;
            if (new_x >= 0 && new_x < m_width) {
                position->x += movement->vx;
            }


            world().component().remove<MovementComponent>(id);
        }

    }

}
