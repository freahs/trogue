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

#ifndef TYRA_SYSTEM_H
#define TYRA_SYSTEM_H

#include "componentmanager.hpp"
#include "componentview.hpp"
#include "typeid.hpp"

#include <bitset>
#include <unordered_set>

namespace tyra {

    struct Component;
    class World;

    class System : protected ComponentView {
    public:
        typedef std::unordered_set<EntityId> Container;

    private:
        Container                           m_updated;
        Container                           m_entities;
        World*                              m_world;

        void addEntity(EntityId);
        void removeEntity(EntityId);

    protected:
        World& world() { return *m_world; }
        const World& world() const { return *m_world; }

        virtual void process(const Container&) { }

        virtual void entityAdded(EntityId) { }
        virtual void entityRemoved(EntityId) { }

    public:

        Container& updated() { return m_updated; }

        virtual ~System() { }

        virtual void init() { }
        void world(World& world) { m_world = &world; }
        void update();
        size_t size() const { return m_entities.size(); }

        void entityUpdated(EntityId, const ComponentSet&);
    };

}

#endif
