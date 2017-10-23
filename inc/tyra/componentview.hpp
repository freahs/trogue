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

#ifndef TYRA_COMPONENTVIEW_H
#define TYRA_COMPONENTVIEW_H

#include <iostream>
#include "config.hpp"
#include "componentset.hpp"
#include "typeid.hpp"

#include <bitset>
#include <cstdint>

namespace tyra {

    class ComponentView {

    public:
        ComponentSet    m_require_one;
        ComponentSet    m_require_all;
        ComponentSet    m_exclude;

    public:
        virtual ~ComponentView() { }
        template<typename T> ComponentView& requireOne();
        template<typename T1, typename T2, typename... Ts> ComponentView& requireOne();
        template<typename T> ComponentView& requireAll();
        template<typename T1, typename T2, typename... Ts> ComponentView& requireAll();
        template<typename T> ComponentView& exclude();
        template<typename T1, typename T2, typename... Ts> ComponentView& exclude();

        bool interested(const ComponentSet&);
        bool operator==(const ComponentView&) const;
        bool operator!=(const ComponentView&) const;

    };

    template<typename T>
    ComponentView& ComponentView::requireOne() {
        TypeId type_id = Type<Component>::id<T>();
        m_require_one.add(type_id);
        return *this;
    }

    template<typename T1, typename T2, typename... Ts>
    ComponentView& ComponentView::requireOne() {
        TypeId type_id = Type<Component>::id<T1>();
        m_require_one.add(type_id);
        requireOne<T2, Ts...>();
        return *this;
    }

    template<typename T>
    ComponentView& ComponentView::requireAll() {
        TypeId type_id = Type<Component>::id<T>();
        m_require_all.add(type_id);
        return *this;
    }

    template<typename T1, typename T2, typename... Ts>
    ComponentView& ComponentView::requireAll() {
        TypeId type_id = Type<Component>::id<T1>();
        m_require_all.add(type_id);
        requireAll<T2, Ts...>();
        return *this;
    }

    template<typename T>
    ComponentView& ComponentView::exclude() {
        TypeId type_id = Type<Component>::id<T>();
        m_exclude.add(type_id);
        return *this;
    }

    template<typename T1, typename T2, typename... Ts>
    ComponentView& ComponentView::exclude() {
        TypeId type_id = Type<Component>::id<T1>();
        m_exclude.add(type_id);
        exclude<T2, Ts...>();
        return *this;
    }
}

#endif
