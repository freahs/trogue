/**
 * Copyright 2017 Fredrik Åhs
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

#ifndef TYRA_COMPONENTSET_H
#define TYRA_COMPONENTSET_H

#include "config.hpp"
#include "typeid.hpp"

#include <bitset>
#include <cstdint>

namespace tyra {

    struct Component;

    class ComponentSet {
    public:
        typedef std::bitset<MAX_COMPONENT_TYPES> container_type;
        container_type m_bits;
        ComponentSet(container_type set) : m_bits(set) {} 

        //template<typename T> static void build(container_type&); 
        //template<typename T1, typename T2, typename... Ts> static void build(container_type&);

    public:
        ComponentSet() = default;
        const container_type& bits() const;
        //template<typename T> static ComponentSet build(); 
        //template<typename T1, typename T2, typename... Ts> static ComponentSet build();

        ComponentSet& add(const TypeId&);
        ComponentSet& remove(const TypeId&);

        bool contains(const TypeId&) const;
        bool contains_all(const ComponentSet& other) const;
        bool contains_any(const ComponentSet& other) const;
        bool contains_none(const ComponentSet& other) const;

        bool empty() const;

        bool operator==(const ComponentSet& other) const;
        bool operator!=(const ComponentSet& other) const;
    };

}

#endif
