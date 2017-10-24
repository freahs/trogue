#ifndef TROGUE_VISIBILITYSYSTEM_H
#define TROGUE_VISIBILITYSYSTEM_H

#include "tyra/tyra.hpp"

#include "scene.hpp"
#include "shadowcast.hpp"

#include "components.hpp"

namespace trogue {

    class VisibilitySystem : public tyra::System {
        int m_height;
        int m_width;

    public:
        VisibilitySystem(int height, int width);
        void process(const tyra::System::Container&) override;
    };
}


#endif
