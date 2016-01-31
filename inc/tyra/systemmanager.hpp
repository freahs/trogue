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

#ifndef LIBTYRA_SYSTEMMANAGER_H
#define LIBTYRA_SYSTEMMANAGER_H

#include "defs.hpp"
#include "typeid.hpp"

#include <vector>

namespace tyra {

    class System;
    class World;

    class SystemManager {
        private:
            std::vector<System*>    m_systems;
            World*                  m_world;

            void add(TypeId, System*);
            System* get(TypeId);

        public:
            SystemManager(World& world) : m_world(&world) { }
            template<typename T, typename... Args> void add(Args&&... args);
            template<typename T> T& get();
            std::vector<System*>& all();
    };

    template<typename T, typename... Args> void SystemManager::add(Args&&... args) {
        static_assert(std::is_base_of<System, T>::value, "SystemManager::addSystem: T must be derived from System.");
        T* ptr = new T{std::forward<Args>(args)...};
        TypeId type_id = Type<System>::id<T>();
        add(type_id, ptr);
    }

    template<typename T> T& SystemManager::get() {
        static_assert(std::is_base_of<System, T>::value, "SystemManager::getSystem: T must be derived from System");
        return static_cast<T&>(*get(Type<System>::id<T>()));
    }

}

#endif
