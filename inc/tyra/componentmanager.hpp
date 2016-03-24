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

#ifndef TYRA_COMPONENTMANAGER_H
#define TYRA_COMPONENTMANAGER_H

#include "entitymanager.hpp"
#include "manager.hpp"
#include "typeid.hpp"

#include <array>
#include <bitset>
#include <cstdint>
#include <unordered_set>
#include <vector>


#include <iostream>

namespace tyra {

    const std::size_t MAX_COMPONENT_TYPES = UINT8_MAX;

    struct Component {
            virtual ~Component() { }
    };

    class ComponentManager : public Manager {
        private:
            typedef std::array<Component*, MAX_COMPONENT_TYPES>	ComponentArray;
            typedef std::bitset<MAX_COMPONENT_TYPES>            ComponentBitSet;

            std::unordered_set<EntityId>    m_updated;

            std::vector<ComponentArray>     m_components;
            std::vector<ComponentBitSet>    m_components_bits;
            size_t                          m_num_registered_components;

            void add(EntityId, TypeId, Component*);
            void remove(EntityId, TypeId);
            void removeAll(EntityId);
            bool valid(EntityId, TypeId) const;
            Component* get(EntityId, TypeId) const;

        public:
            ~ComponentManager();

            template <typename T> bool valid(EntityId) const;
            template <typename T> T& get(EntityId) const;
            template <typename T, typename... Args> void add(EntityId, Args&&...);
            template <typename T> void remove(EntityId);

            std::bitset<MAX_COMPONENT_TYPES>& bits(EntityId); 
            std::unordered_set<EntityId>& updated() { return m_updated; }
            size_t size() const { return m_num_registered_components; }
    };

    template <typename T> bool ComponentManager::valid(EntityId entity_id) const {
        static_assert(std::is_base_of<Component, T>::value, "ComponentManager::valid: T must be derived from Component");
        TypeId type_id = Type<Component>::id<T>();
        return valid(entity_id, type_id);
    }

    template <typename T> T& ComponentManager::get(EntityId entity_id) const {
        static_assert(std::is_base_of<Component, T>::value, "ComponentManager::get: T must be derived from Component");
        TypeId type_id = Type<Component>::id<T>();
        Component* component_ptr = get(entity_id, type_id);
        return static_cast<T&>(*component_ptr);
    }

    template <typename T, typename... Args>	void ComponentManager::add(EntityId entity_id, Args&&... args)	{
        static_assert(std::is_base_of<Component, T>::value, "ComponentManager::add: T must be derived from Component");
        T* component_ptr = new T{std::forward<Args>(args)...};
        TypeId type_id = Type<Component>::id<T>();
        add(entity_id, type_id, component_ptr);
    }

    template <typename T> void ComponentManager::remove(EntityId entity_id) {
        static_assert(std::is_base_of<Component, T>::value, "ComponentManager::remove: T must be derived from Component");
        TypeId type_id = Type<Component>::id<T>();
        remove(entity_id, type_id); 
    }

}

#endif
