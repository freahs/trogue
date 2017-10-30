#include "tilesystem.hpp"

#include "components.hpp"
#include "ansi_format.hpp"
#include "xterm_display.hpp"


namespace trogue {

    TileSystem::TileSystem(Display& display, int height, int width)
    : m_display(&display), m_height(height), m_width(width) {
        requireAll<PositionComponent, TileComponent>();
    }

    Display& TileSystem::display() {
        return *m_display;
    }

    void TileSystem::process(const tyra::System::Container&) {
        int disp_half_height = display().height()/2;
        int disp_half_width  = display().width()/2;


        auto pid = world().tag("PLAYER");
        auto player_pos = world().component().get<PositionComponent>(pid);

        world().scene().update(world().delta());
        for(int rel_y = -disp_half_height; rel_y <= disp_half_height; ++rel_y) {
            int scene_y =  player_pos.y + rel_y;
            for (int rel_x = -disp_half_width; rel_x <= disp_half_width; ++rel_x) {
                int scene_x = player_pos.x + rel_x;
                auto tile = &Tile::blank();
                if (world().scene().inRange(scene_y, scene_x)) {
                    tyra::EntityId id = world().scene().get(scene_y, scene_x);
                    if (world().scene().all(scene_y, scene_x).size() > 1) {
                    }
                    if (id != tyra::EntityId(-1)) {
                        auto tc = &world().component().get<TileComponent>(id);
                        if (world().component().valid<VisibleComponent>(id)) {
                            tile = &tc->normal();
                        } else if (world().component().valid<ShadowComponent>(id)) {
                            tile = &tc->blocked();
                        }
                    }
                }
                display().set(disp_half_height + rel_y, disp_half_width + rel_x, tile->symbol, tile->color, tile->bg_color);
            }
        }
        display().draw(std::cout);
    }

    void TileSystem::entityAdded(tyra::EntityId id) {
    }

    void TileSystem::entityRemoved(tyra::EntityId id) {
    }

}
