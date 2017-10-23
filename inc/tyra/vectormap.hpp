#ifndef TROGUE_VECTORMAP_H
#define TROGUE_VECTORMAP_H

#include <unordered_map>
#include <vector>

namespace tyra {

    template<class T> class VectorMap;
    template<class T> void swap(VectorMap<T>&, VectorMap<T>&);

    template<class T> class VectorMap {
    public:
        typedef typename std::vector<T>::iterator       iterator;
        typedef typename std::vector<T>::const_iterator const_iterator;
        typedef typename std::vector<T>::value_type     value_type;
        typedef typename std::vector<T>::size_type      size_type;

    private:
        std::vector<T>                      m_vector;
        std::unordered_map<T, size_type>    m_map;

    public:

        VectorMap() = default;
        VectorMap(const VectorMap<T>& other);
        VectorMap(VectorMap<T>&& other);

        VectorMap<T>& operator=(VectorMap<T> other);

        iterator begin();
        iterator end();
        iterator find(const T& val);
        const_iterator begin() const;
        const_iterator end() const;
        const_iterator find(const T& val) const;
        bool contains(const T& val) const;

        bool insert(const T& val);
        bool erase(const T& val);

        size_type size() const;

        friend void swap<>(VectorMap<T>&, VectorMap<T>&);
    };

    template<class T>
    VectorMap<T>::VectorMap(const VectorMap<T>& other) {
        m_vector = other.m_vector;
        m_map = other.m_map;
    }

    template<class T> 
    VectorMap<T>::VectorMap(VectorMap<T>&& other) {
        std::swap(*this, other);
    }

    template<class T>
    VectorMap<T>&
    VectorMap<T>::operator=(VectorMap<T> other) {
        swap(*this, other);
        return *this;
    }

    template<class T>
    typename VectorMap<T>::iterator
    VectorMap<T>::begin() {
        return m_vector.begin();
    }

    template<class T>
    typename VectorMap<T>::iterator
    VectorMap<T>::end() {
        return m_vector.end();
    }

    template<class T>
    typename VectorMap<T>::iterator
    VectorMap<T>::find(const T& val) {
        if (contains(val)) {
            auto it = m_vector.begin();
            std::advance(it, m_map.find(val)->second);
            return it;
        }
        return m_vector.end();
    }

    template<class T>
    typename VectorMap<T>::const_iterator
    VectorMap<T>::begin() const {
        return m_vector.begin();
    }

    template<class T>
    typename VectorMap<T>::const_iterator
    VectorMap<T>::end() const {
        return m_vector.end();
    }

    template<class T>
    typename VectorMap<T>::const_iterator
    VectorMap<T>::find(const T& val) const {
        if (contains(val)) {
            auto it = m_vector.begin();
            std::advance(it, m_map.find(val)->second);
            return it;
        }
        return m_vector.end();
    }

    template<class T>
    bool
    VectorMap<T>::contains(const T& val) const {
        return m_map.find(val) != m_map.end();
    }

    template<class T>
    bool
    VectorMap<T>::insert(const T& val) {
        if (!contains(val)) {
            auto pos = m_vector.size();
            m_map[val] = pos;
            m_vector.push_back(val);
            return true;
        }
        return false;
    }

    template<class T>
    bool
    VectorMap<T>::erase(const T& val) {
        if (contains(val)) {
            auto pos = m_map[val];
            std::swap(m_vector[pos], m_vector[m_vector.size() - 1]);
            m_vector.pop_back();
            return true;
        }
        return false;
    }

    template<class T>
    typename VectorMap<T>::size_type
    VectorMap<T>::size() const {
        return m_vector.size();
    }

    template<class T>
    void
    swap(VectorMap<T>& first, VectorMap<T>& second) {
        std::swap(first.m_vector, second.m_vector);
        std::swap(first.m_map, second.m_map);
    }

}

#endif
