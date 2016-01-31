#ifndef LIBTYRA_EVENTMANAGER_H
#define LIBTYRA_EVENTMANAGER_H

#include "defs.hpp"
#include "entitymanager.hpp"
#include "typeid.hpp"

#include <map>
#include <set>

namespace tyra {

    class Event {
    public:
        virtual void run(EntityId) const { }
    };

    class EventManager {
    private:
        std::map<TypeId, std::set<EntityId>> m_events;

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
