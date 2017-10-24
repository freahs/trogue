#include "tilesystem.hpp"


#include "components.hpp"
#include "ansi_format.hpp"
#include "xterm_display.hpp"

#include "map.hpp"

#include <algorithm>


namespace trogue {

    TileSystem::TileSystem(Display& display, int height, int width)
    : m_rotator_map(height, width), m_display(&display), m_height(height), m_width(width) {
        requireAll<RenderComponent>();
    }

    Display& TileSystem::display() {
        return *m_display;
    }

    void TileSystem::process(const tyra::System::Container& ids) {

        EntityMap m(m_height, m_width);

        // TODO: Currently only renders the entities in the highest layer. Should combine apply
        // bg_color from lower layers if applicable (or vice versa)
        for (auto id : ids) {
            auto rc = &world().component().get<RenderComponent>(id);
            auto vec  = &m[rc->y][rc->x];
            if(vec->empty()) {
                vec->push_back(id);
            } else {
                auto l1 = world().component().get<RenderComponent>(id).layer;
                auto l2 = world().component().get<RenderComponent>(vec->back()).layer;

                if (l1 == l2) {
                    vec->push_back(id);
                } else if (l1 > l2) {
                    vec->clear();
                    vec->push_back(id);
                }
            }
        }

        for (auto& vec : m) {
            std::sort(vec.begin(), vec.end());
        }

        int disp_half_height = display().height()/2;
        int disp_half_width  = display().width()/2;

        auto pid = world().tag("PLAYER");
        auto center_y = world().component().get<PositionComponent>(pid).y;
        auto center_x = world().component().get<PositionComponent>(pid).x;


        for(int rel_y = -disp_half_height; rel_y <= disp_half_height; ++rel_y) {
            int scene_y =  center_y + rel_y;
            for (int rel_x = -disp_half_width; rel_x <= disp_half_width; ++rel_x) {
                int scene_x = center_x + rel_x;
                std::string symbol = " ";
                int color = -1;
                int bg_color = -1;
                if (m.inRange(scene_y, scene_x) && !m[scene_y][scene_x].empty()) {
                    //TODO: change to rotation
                    auto id = m[scene_y][scene_x].back();
                    auto rc = world().component().get<RenderComponent>(id);
                    symbol = rc.symbol;
                    color = rc.color;
                    bg_color = rc.bg_color;
                }
                auto disp_y = disp_half_height + rel_y;
                auto disp_x = disp_half_width + rel_x;
                display().set(disp_y, disp_x, symbol, color, bg_color);
            }
        }
        display().draw(std::cout);
    }

}
