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

#ifndef LIBTYRA_ENTITYMANAGER_H
#define LIBTYRA_ENTITYMANAGER_H

#include "../inc/defs.hpp"

#include <unordered_set>
#include <vector>


namespace tyra {

    class EntityManager {

        std::vector<EntityVersion>      m_pool;
        std::vector<EntityIndex>        m_free;
        std::unordered_set<EntityIndex> m_active;

        public:
        EntityIndex static index(EntityId id )                 { return id & 0xFFFF; }
        EntityVersion static version(EntityId id)              { return id >> 16; }
        EntityId static id(EntityVersion ver, EntityIndex idx) { return (ver << 16) | idx; }

        bool valid(EntityId) const;

        EntityId create();
        void destroy(EntityId);

        bool active(EntityId) const;
        void activate(EntityId);
        void deactivate(EntityId);
    };

}

#endif
