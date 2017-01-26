#ifndef TROGUE_MAP_H
#define TROGUE_MAP_H

#include <vector>
#include <type_traits>
#include <iostream>

namespace trogue {

    template<typename T> class Map {

    public:
        static_assert(std::is_default_constructible<T>::value, "T must be default constructable");

        typedef int size_type;
        typedef typename std::vector<T>::iterator iterator;
        typedef typename std::vector<T>::const_iterator const_iterator;
        typedef typename std::vector<T>::reference reference;
        typedef typename std::vector<T>::const_reference const_reference;

    private:
        std::vector<T>  m_elements;
        size_type       m_height;
        size_type       m_width;

        class Proxy {
            std::vector<T>* m_vec;
            const size_type  m_offset;

        public:
            Proxy(std::vector<T> &vec, const size_type offset) : m_vec(&vec), m_offset(offset) { }
            reference operator[](const size_type x) { return (*m_vec)[m_offset + x]; }
        };

        class ConstProxy {
            const std::vector<T>* m_vec;
            const size_type  m_offset;

        public:
            ConstProxy(const std::vector<T> &vec, const size_type offset) : m_vec(&vec), m_offset(offset) { }
            const_reference operator[](const size_type x) const { return (*m_vec)[m_offset + x]; }
        };

    public:
        Map(size_type height, size_type width)
            : m_elements(height*width)
            , m_height(height)
            , m_width(width) {
              }

        const size_type width() const {
            return m_width;
        }

        const size_type height() const {
            return m_height;
        }

        const bool inRange(size_type y, size_type x) const {
            return y >= 0 && y < height() && x >= 0 && x < width();
        }

        Proxy operator[](size_type y) {
            return Proxy(m_elements, y*width());
        }

        const ConstProxy operator[](size_type y) const {
            return ConstProxy(m_elements, y*width());
        }

        iterator begin() {
            return m_elements.begin();
        }

        iterator end() {
            return m_elements.end();
        }

        const_iterator begin() const {
            return m_elements.begin();
        }

        const_iterator end() const {
            return m_elements.end();
        }

        void clear() {
            std::fill(m_elements.begin(), m_elements.end(), {});
        }

    };

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Map<T>& m)
    {
        for (auto y = 0; y < m.height(); ++y) {
            for (auto x = 0; x < m.width(); ++x) {
                os << m[y][x];
            }
            os << std::endl;
        }
        return os;
    }
}

#endif
