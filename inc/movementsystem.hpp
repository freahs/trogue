#ifndef TROGUE_MOVEMENTSYSTEM_H
#define TROGUE_MOVEMENTSYSTEM_H

#include "tyra/tyra.hpp"
#include "ecs.hpp"

#include "scene.hpp"

#include <unordered_set>

namespace trogue {

     class MovementSystem : public TSystem {
     public:
         MovementSystem();
         void process(const tyra::System::Container& ids) override;
     };

}

#endif
