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

#ifndef TYRA_EVENTMANAGER_H
#define TYRA_EVENTMANAGER_H

#include "config.hpp"
#include "manager.hpp"
#include "typeid.hpp"

#include <map>
#include <unordered_set>

namespace tyra {


    class Event {
    public:
        virtual void run(EntityId) const { }
    };

    class EventManager : public Manager {
    private:
        std::map<TypeId, std::unordered_set<EntityId>> m_events;

        void add(EntityId, TypeId);
        void remove(EntityId, TypeId);
        void create(TypeId, Event&);

    public:
        template <typename T> void add(EntityId);
        template <typename T> void remove(EntityId);
        template <typename T, typename...Args> void create(Args&&...);
    };

    template <typename T> void EventManager::add(EntityId id) {
        static_assert(std::is_base_of<Event, T>::value, "EventManager::add: T must be derived from Event");
        add(id, Type<Event>::id<T>());
    }

    template <typename T> void EventManager::remove(EntityId id) {
        static_assert(std::is_base_of<Event, T>::value, "EventManager::remove: T must be derived from Event");
        remove(id, Type<Event>::id<T>());
    }

    template <typename T, typename... Args> void EventManager::create(Args&&... args) {
        static_assert(std::is_base_of<Event, T>::value, "EventManager::remove: T must be derived from Event");
        T event{std::forward<Args>(args)...};
        create(Type<Event>::id<T>(), event);
    }

}


#endif
