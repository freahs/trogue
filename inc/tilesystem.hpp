#ifndef TROGUE_TILESYSTEM_H
#define TROGUE_TILESYSTEM_H

#include "tyra/tyra.hpp"

#include "scene.hpp"
#include "xterm_display.hpp"

namespace trogue {

    class TileSystem : public tyra::System {
    private:
        Scene*      m_scene;
        Display*    m_display;
        int         m_height;
        int         m_width;
        Scene&      scene();
        Display&    display();
    public:
        TileSystem(Scene& scene, Display& display, int height, int width);
        void process(const tyra::System::Container& ids) override;
        void entityAdded(tyra::EntityId id) override;
        void entityRemoved(tyra::EntityId id) override;
    };

}

#endif
