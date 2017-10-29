#ifndef TROGUE_VISIBILITYSYSTEM_H
#define TROGUE_VISIBILITYSYSTEM_H

#include "tyra/tyra.hpp"
#include "ecs.hpp"

#include "scene.hpp"
#include "shadowcast.hpp"

#include "components.hpp"

namespace trogue {

    class VisibilitySystem : public TSystem {
    private:

    public:
        VisibilitySystem();
        void process(const tyra::System::Container&) override;
    };
}


#endif
