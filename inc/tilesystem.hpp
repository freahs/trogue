#ifndef TROGUE_TILESYSTEM_H
#define TROGUE_TILESYSTEM_H

#include <vector>

#include "tyra/tyra.hpp"

#include "map.hpp"

#include "scene.hpp"
#include "xterm_display.hpp"


namespace trogue {

    class TileSystem : public tyra::System {
    private:

        typedef Map<std::vector<tyra::EntityId>> EntityMap;

        Map<int>    m_rotator_map;
        Display*    m_display;
        int         m_height;
        int         m_width;
        Display&    display();
    public:
        TileSystem(Display& display, int height, int width);
        void process(const tyra::System::Container& ids) override;
    };

}

#endif
