#include "tilesystem.hpp"

#include "components.hpp"
#include "ansi_format.hpp"
#include "xterm_display.hpp"


namespace trogue {

    TileSystem::TileSystem(Scene& scene, Display& display, int height, int width)
    : m_scene(&scene), m_display(&display), 
    m_height(height), m_width(width) {
        requireAll<PositionComponent>();
        requireAll<TileComponent>();
    }

    Scene& TileSystem::scene() {
        return *m_scene;
    }

    Display& TileSystem::display() {
        return *m_display;
    }

    void TileSystem::process(const tyra::System::Container&) {
        int disp_half_height = display().height()/2;
        int disp_half_width  = display().width()/2;


        for(int rel_y = -disp_half_height; rel_y <= disp_half_height; ++rel_y) {
            int scene_y =  scene().centerY() + rel_y;
            for (int rel_x = -disp_half_width; rel_x <= disp_half_width; ++rel_x) {
                int scene_x = scene().centerX() + rel_x;
                auto tile = &Tile::blank();
                if (scene().inRange(scene_y, scene_x)) {
                    tyra::EntityId id = scene().get(scene_y, scene_x);
                    if (id != tyra::EntityId(-1)) {
                        auto tc = world().component().get<TileComponent>(id);
                        if (scene().visible(scene_y, scene_x)) {
                            tile = &tc.normal();
                        } else if (scene().visited(scene_y, scene_x)) {
                            tile = &tc.blocked();
                        }
                    }
                }
                display().set(disp_half_height + rel_y, disp_half_width + rel_x, tile->symbol, tile->color, tile->bg_color);
            }
        }
        display().draw(std::cout);
    }

    void TileSystem::entityAdded(tyra::EntityId id) {
        auto position = world().component().get<PositionComponent>(id);
        auto tile = world().component().get<TileComponent>(id);
        scene().add(position.y, position.x, tile.layer, id);
    }

    void TileSystem::entityRemoved(tyra::EntityId id) {
        auto position = world().component().get<PositionComponent>(id);
        auto tile = world().component().get<TileComponent>(id);
        scene().remove(position.y, position.x, id);
    }

}
