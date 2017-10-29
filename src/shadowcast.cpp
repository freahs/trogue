#include "shadowcast.hpp"
#include <iostream>

namespace trogue {

    const Shadowcast::Mod Shadowcast::mods[8] = {
        {1,0,0,1}, {0,1,1,0}, {0,-1,1,0}, {-1,0,0,1},
        {-1,0,0,-1}, {0,-1,-1,0}, {0,1,-1,0}, {1,0,0,-1}
    };

    Shadowcast::Shadowcast(const Map<bool>& opaque_map) :
        m_opaque(opaque_map),
        m_visible(opaque_map.height(), opaque_map.width()),
        m_visited(opaque_map.height(), opaque_map.width()) {
        }
    
    void Shadowcast::cast(int center_x, int center_y, int radius, int row, float start_slope, float stop_slope, const Mod& m) {
        if (start_slope < stop_slope) {
            return;
        }
        float next_start_slope = start_slope;

        // iterate rows, center out 
        for (int y = -row; y >= -radius; y--) {
            bool blocked = false;

            // iterate rows left to right.
            for (int x = y; x <= 0; x++) {

                float l_slope = (x - 0.5) / (y + 0.5);
                float r_slope = (x + 0.5) / (y - 0.5);

                // this would indicate that 
                if (l_slope <= stop_slope) {
                    break;
                }

                int abs_y = center_y + x*m.yx + y*m.yy;
                int abs_x = center_x + x*m.xx + y*m.xy;
                if (start_slope >= r_slope && m_opaque.inRange(abs_y, abs_x) && x*x + y*y < radius*radius) {
                    m_visible[abs_y][abs_x] = true;
                    m_visited[abs_y][abs_x] = true;

                    if (blocked && m_opaque[abs_y][abs_x]) {
                        next_start_slope = r_slope;
                    } else if (blocked) {
                        blocked = false;
                        start_slope = next_start_slope;
                    } else if (m_opaque[abs_y][abs_x]) {
                        blocked = true;
                        next_start_slope = r_slope;
                        cast(center_x, center_y, radius, 1 - y, start_slope, l_slope, m);
                    }
                }
            }
            if (blocked) {
                break;
            }
        }
    }

    void Shadowcast::update(int center_y, int center_x, int radius) {
        m_visible.clear();
        for (auto i = 0; i < 8; ++i) {
            cast(center_x, center_y, radius, 1, 1.0, 0.0, mods[i]);
        }
    }

    bool Shadowcast::visible(int y, int x) const {
        return m_visible[y][x];
    }

    bool Shadowcast::visited(int y, int x) const {
        return m_visited[y][x];
    }

    void Shadowcast::set(int y, int x, bool opaque) {
        m_opaque[y][x] = opaque;
    }
}
