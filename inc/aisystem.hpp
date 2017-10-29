#ifndef TROGUE_AISYSTEM_H
#define TROGUE_AISYSTEM_H

#include "tyra/tyra.hpp"
#include "ecs.hpp"

#include "scene.hpp"

namespace trogue {

    class AISystem : public TSystem {
    public:
        AISystem();
        void process(const tyra::System::Container&) override;
    };
}


#endif
