#ifndef TROGUE_SHADOWCAST_H
#define TROGUE_SHADOWCAST_H

#include "map.hpp"


namespace trogue {

    class Shadowcast {

        struct Mod {
            const int xx;
            const int xy;
            const int yx;
            const int yy;
        };

        const static Mod mods[8];
    public:

        const Map<bool>&    m_terrain_map;
        Map<bool>           m_visible_map;
        Map<bool>           m_visited_map;

        void cast(int x, int y, int radius, int row, float start_slope, float end_slope, const Mod& m);

    public:
        Shadowcast(const Map<bool> &terrain_map);
        void update(int y, int x, int radius);

        bool visible(int y, int x) const;
        bool visited(int y, int x) const;
    };
}

#endif
