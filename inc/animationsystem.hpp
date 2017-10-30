#ifndef TROGUE_ANIMATIONSYSTEM_H
#define TROGUE_ANIMATIONSYSTEM_H

#include "ecs.hpp"


namespace trogue {

    class AnimationSystem : public TSystem {
    private:

    public:
        AnimationSystem();
        void process(const tyra::System::Container& ids) override;
        //void entityAdded(tyra::EntityId id) override;
        //void entityRemoved(tyra::EntityId id) override;
    };

}
#endif
