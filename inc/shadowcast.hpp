#ifndef TROGUE_SHADOWCAST_H
#define TROGUE_SHADOWCAST_H

#include "map.hpp"


namespace trogue {

    class Shadowcast {
    private:
        struct Mod {
            const int xx;
            const int xy;
            const int yx;
            const int yy;
        };

        const static Mod mods[8];
        Map<int> m_num_opaque;
        Map<bool> m_visible;
        Map<bool> m_visited;

        void cast(int x, int y, int radius, int row, float start_slope, float end_slope, const Mod& m);

    public:
        Shadowcast(const Map<int> &terrain_map);
        Shadowcast(int height, int width);
        void update(int y, int x, int radius);

        bool visible(int y, int x) const;
        void visible(int y, int x, bool visible);
        bool visited(int y, int x) const;

        bool opaque(int y, int x) const;
        void opaque(int y, int x, bool opaque);
        int height() const;
        int width() const;
        bool inRange(int y, int x) const;

    };
}

#endif
