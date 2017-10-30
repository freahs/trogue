#include "entitystack.hpp"
#include <algorithm>

namespace trogue {
    EntityStack::EntityStack()
    : m_current(m_items.end()) {
    }

    EntityStack::iterator EntityStack::begin() {
        return iterator(m_items.begin());
    }

    EntityStack::iterator EntityStack::end() {
        return iterator(m_items.end());
    }

    EntityStack::const_iterator EntityStack::begin() const {
        return const_iterator(m_items.begin());
    }

    EntityStack::const_iterator EntityStack::end() const {
        return const_iterator(m_items.end());
    }
    void EntityStack::rotate() {
        if (m_items.size() != 0) {
            ++m_current;
        }
    }

    void EntityStack::update() {
        if (m_items.size() != 0) {
            if (m_current == m_items.end()) {
                m_current = m_items.begin();
            } else if (m_current->second < m_items.begin()->second) {
                m_current = m_items.begin();
            }
        }
    }

    bool EntityStack::add(const data_type& v, const meta_type& m) {
        auto res = m_items.insert({v, m});
        m_current = res.first;
        return res.second;
    }

    bool EntityStack::remove(const data_type& v) {
        if (m_current != m_items.end()) {
            auto res = std::find_if(m_items.begin(), m_items.end(),
                                    [&](const pair_type& p) { return p.first == v; }
                                   );

            if (res == m_items.end()) { return false; }
            auto next = m_items.erase(res);
            if (m_current == res) { m_current = next; } 
            if (m_current == m_items.end()) { m_current = m_items.begin(); }
            return true;
        }
        return false;
    }

    EntityStack::data_type EntityStack::get() const {
        if (m_current == m_items.end()) {
            return -1;
        } else {
            return m_current->first;
        }
    }
}
