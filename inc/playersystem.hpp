#ifndef TROGUE_PLAYERSYSTEM_H
#define TROGUE_PLAYERSYSTEM_H

#include "tyra/tyra.hpp"
#include "ecs.hpp"

#include "scene.hpp"

namespace trogue {

    class PlayerSystem : public TSystem {
    public:
        PlayerSystem();
        void process(const tyra::System::Container&) override;
    };
}


#endif
