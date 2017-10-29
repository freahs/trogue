#ifndef TROGUE_TILESYSTEM_H
#define TROGUE_TILESYSTEM_H

#include "tyra/tyra.hpp"
#include "ecs.hpp"

#include "scene.hpp"
#include "xterm_display.hpp"

namespace trogue {

    class TileSystem : public TSystem {
    private:
        Display*    m_display;
        int         m_height;
        int         m_width;
        Display&    display();

    public:
        TileSystem(Display& display, int height, int width);
        void process(const tyra::System::Container& ids) override;
        void entityAdded(tyra::EntityId id) override;
        void entityRemoved(tyra::EntityId id) override;
    };

}

#endif
