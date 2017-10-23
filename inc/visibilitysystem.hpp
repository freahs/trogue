#ifndef TROGUE_VISIBILITYSYSTEM_H
#define TROGUE_VISIBILITYSYSTEM_H

#include "tyra/tyra.hpp"

#include "scene.hpp"
#include "shadowcast.hpp"

#include "components.hpp"

namespace trogue {

    class VisibilitySystem : public tyra::System {
    private:
        Scene*  m_scene;
        Scene& scene();

    public:
        VisibilitySystem(Scene& scene);
        void process(const tyra::System::Container&) override;
    };
}


#endif
