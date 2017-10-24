#ifndef TROGUE_PLAYERSYSTEM_H
#define TROGUE_PLAYERSYSTEM_H

#include "tyra/tyra.hpp"

#include "scene.hpp"

namespace trogue {

    class PlayerSystem : public tyra::System {
    private:
        Scene*  m_scene;
        Scene& scene();

    public:
        PlayerSystem();
        void process(const tyra::System::Container&) override;
    };
}


#endif
