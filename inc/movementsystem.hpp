#ifndef TROGUE_MOVEMENTSYSTEM_H
#define TROGUE_MOVEMENTSYSTEM_H

#include "tyra/tyra.hpp"

#include "scene.hpp"

#include <unordered_set>

namespace trogue {

     class MovementSystem : public tyra::System {
     private:
         Scene* m_scene;
         Scene& scene();

     public:
         void process(const tyra::System::Container& ids) override;
         MovementSystem(Scene& scene);
     };

}

#endif
