#ifndef TROGUE_ECS_H
#define TROGUE_ECS_H

#include "tyra/tyra.hpp"
#include "scene.hpp"

namespace trogue {
    class TWorld : public tyra::World {
    private:
        Scene m_scene;

    public:
        TWorld(int height, int width) : tyra::World(), m_scene(height, width){ }
        Scene& scene() { return m_scene; }
        const Scene& scene() const { return m_scene; }

    };
}

namespace trogue {
    class TSystem : public tyra::System {
    public:
        using tyra::System::System;
        TWorld& world() { return static_cast<TWorld&>(tyra::System::world()); }
        const TWorld& world() const { return static_cast<const TWorld&>(tyra::System::world()); }
    };
}

#endif
