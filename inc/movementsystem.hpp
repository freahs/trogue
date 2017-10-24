#ifndef TROGUE_MOVEMENTSYSTEM_H
#define TROGUE_MOVEMENTSYSTEM_H

#include "tyra/tyra.hpp"

#include "scene.hpp"

#include <unordered_set>

namespace trogue {

     class MovementSystem : public tyra::System {
     private:
         int m_height, m_width;

     public:
         void process(const tyra::System::Container& ids) override;
         MovementSystem(int height, int width);
     };

}

#endif
