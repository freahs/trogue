#include "movementsystem.hpp"

#include "components.hpp"

#include "tyra/tyra.hpp"

#include <unordered_set>


namespace trogue {

    MovementSystem::MovementSystem() {
        requireAll<TileComponent, PositionComponent, MovementComponent>();
    }

    void MovementSystem::process(const tyra::System::Container& ids) {

        for (tyra::EntityId id : ids) {
            auto tile = &world().component().get<TileComponent>(id);
            auto position = &world().component().get<PositionComponent>(id);
            auto movement = &world().component().get<MovementComponent>(id);

            auto new_y = position->y + movement->vy;
            auto new_x = position->x + movement->vx;

            auto visible = world().component().valid<VisibleComponent>(id);
            auto opaque = false;
            if (world().component().valid<AttributeComponent>(id)) {
                if (world().component().get<AttributeComponent>(id).has(Attribute::OPAQUE)) {
                    opaque = true;
                }
            }

            if (world().scene().inRange(new_y, new_x)) {
                bool blocked = false;
                for (auto id : world().scene().all(new_y, new_x)) {
                    if (world().component().valid<AttributeComponent>(id)) {
                        if (world().component().get<AttributeComponent>(id).has(Attribute::SOLID)) {
                            if (!world().component().valid<CollisionComponent>(id)) {
                                world().component().add<CollisionComponent>(id);
                            }
                            world().component().get<CollisionComponent>(id).add(id);
                            blocked = true;
                            break;
                        }
                    }
                }
                if(!blocked) {
                    if (visible) { world().scene().remove(position->y, position->x, id); }
                    //if (opaque) { world().scene().opaque(position->y, position->x, false); }
                    position->y += movement->vy;
                    position->x += movement->vx;
                    if (visible) { world().scene().add(position->y, position->x, tile->layer, id); }
                    //if (opaque) { world().scene().opaque(position->y, position->x, true); }
                }
            }

            world().component().remove<MovementComponent>(id);
        }

    }

}
