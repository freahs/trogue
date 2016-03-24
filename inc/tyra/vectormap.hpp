#ifndef TROGUE_VECTORMAP_H
#define TROGUE_VECTORMAP_H

#include <unordered_map>
#include <vector>

namespace tyra {

    template<class T> class VectorMap {
    private:
        std::vector<T>                  m_vector;
        std::unordered_map<T, size_t>   m_map;

    public:
        typedef typename std::vector<T>::const_iterator const_iterator;
        typedef typename std::vector<T>::value_type     value_type;

        VectorMap() = default;

        const_iterator begin() const;
        const_iterator end() const;
        const_iterator find(const T& val) const;
        bool contains(const T& val) const;

        bool insert(const T& val);
        bool erase(const T& val);

        size_t size() const;
    };

    template<class T> typename VectorMap<T>::const_iterator VectorMap<T>::begin() const {
        return m_vector.begin();
    }

    template<class T> typename VectorMap<T>::const_iterator VectorMap<T>::end() const {
        return m_vector.end();
    }

    template<class T> typename VectorMap<T>::const_iterator VectorMap<T>::find(const T& val) const {
        if (contains(val)) {
            auto it = m_vector.begin() + m_map.find(val)->second;
            return it;
        }
        return m_vector.end();
    }

    template<class T> bool VectorMap<T>::contains(const T& val) const {
        return m_map.find(val) != m_map.end();
    }

    template<class T> bool VectorMap<T>::insert(const T& val) {
        if (!contains(val)) {
            int pos = m_vector.size();
            m_map[val] = pos;
            m_vector.push_back(val);
            return true;
        }
        return false;
    }

    template<class T> bool VectorMap<T>::erase(const T& val) {
        if (contains(val)) {
            int pos = m_map[val];
            std::swap(m_vector[pos], m_vector[m_vector.size() - 1]);
            m_vector.pop_back();
            return true;
        }
        return false;
    }

    template<class T> size_t VectorMap<T>::size() const {
        return m_vector.size();
    }

}

#endif
