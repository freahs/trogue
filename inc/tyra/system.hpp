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
#include "typeid.hpp"

#include <bitset>
#include <unordered_set>

namespace tyra {

    class Component;
    class World;

    class System {
        private:
            std::bitset<MAX_COMPONENT_TYPES>    m_require_one_components;
            std::bitset<MAX_COMPONENT_TYPES>	m_require_all_components;
            std::bitset<MAX_COMPONENT_TYPES>	m_exclude_components;
            std::unordered_set<EntityId>		m_entities;
            World*								m_world;

            void requireOne(TypeId);
            void requireAll(TypeId);
            void exclude(TypeId);

            void addEntity(TypeId);
            void removeEntity(TypeId);

        protected:
            World& world() const { return *m_world; }
            template<typename T> void requireOne();
            template<typename T> void requireAll();
            template<typename T> void exclude();

            virtual void process(const std::unordered_set<EntityId>&) { }

            virtual void entityAdded(EntityId) { }
            virtual void entityRemoved(EntityId) { }

        public:
            virtual ~System() { }

            void world(World& world) { m_world = &world; }
            void update();
            size_t size() const { return m_entities.size(); }

            void entityUpdated(EntityId, std::bitset<MAX_COMPONENT_TYPES>&);
    };

    template<typename T> void System::requireOne() {
        TypeId id = Type<Component>::id<T>();
        requireOne(id);
    }

    template<typename T> void System::requireAll() {
        TypeId id = Type<Component>::id<T>();
        requireAll(id);
    }

    template<typename T> void System::exclude() {
        TypeId id = Type<Component>::id<T>();
        exclude(id);
    }

}

#endif
