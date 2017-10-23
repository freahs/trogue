#ifndef TROGUE_AISYSTEM_H
#define TROGUE_AISYSTEM_H

#include "tyra/tyra.hpp"

#include "scene.hpp"

namespace trogue {

    class AISystem : public tyra::System {
    public:
        AISystem();
        void process(const tyra::System::Container&) override;
    };
}


#endif
