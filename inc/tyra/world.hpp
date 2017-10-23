/**
 * Copyright 2016 Fredrik Åhs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TYRA_WORLD_H
#define TYRA_WORLD_H

#include "config.hpp"

#include <chrono>
#include <map>

namespace tyra {

    class ComponentManager;
    class EntityManager;
    class SystemManager;

    class World {
    private:
        typedef std::chrono::high_resolution_clock      Time;
        typedef std::chrono::milliseconds               Ms;
        typedef std::chrono::system_clock::time_point   TimePoint;


        ComponentManager*   m_component_manager;
        EntityManager*      m_entity_manager;
        SystemManager*      m_system_manager;
        bool                m_processing;
        TimePoint           m_prev_update;
        int                 m_delta;

        std::map<std::string, EntityId> m_tags;

    protected:
        virtual void preUpdate() { }
        virtual void postUpdate() { }

    public:
        World();
        ~World();

        void start()            { m_processing = true; }
        void stop()             { m_processing = false; }
        bool processing() const { return m_processing; }
        void update();

        EntityManager& entity();
        ComponentManager& component();
        SystemManager& system();

        EntityId tag(const std::string&) const;
        void tag(const std::string&, EntityId);
        int delta() const;
    };

}


#endif
