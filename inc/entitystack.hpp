#ifndef TROGUE_ENTITYSTACK_H
#define TROGUE_ENTITYSTACK_H
#include <iterator>
#include <utility>
#include <set>
#include "tyra/tyra.hpp"

namespace trogue {

    class EntityStack {
    private:
        using data_type = tyra::EntityId;
        using meta_type = int;
        using pair_type = typename std::pair<data_type, meta_type>;
        struct cmp {
            bool operator()(const pair_type& p1, const pair_type& p2) const {
                if (p1.second == p2.second) {
                    return p1.first < p2.first;
                }
                return p1.second > p2.second;
            }
        };
        using container = typename std::set<pair_type, cmp>;
        template<typename parent_iterator> class base_iterator;
        container m_items;
        container::const_iterator m_current;

    public:
        //class iterator;
        class const_iterator;
        using iterator = const_iterator;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

        EntityStack();
        void update();
        bool add(const data_type&);
        bool add(const data_type&, const meta_type&);
        bool remove(const data_type&);
        data_type get() const;
    };


    template<typename parent_iterator>
    class EntityStack::base_iterator : std::iterator<std::random_access_iterator_tag, data_type> {
    protected:
        parent_iterator _ptr;
    public:
        using difference_type = typename std::iterator<std::random_access_iterator_tag, data_type>::difference_type;

        base_iterator() : _ptr(nullptr) {}
        base_iterator(parent_iterator itr) : _ptr(itr) {}
        base_iterator(const base_iterator &rhs) : _ptr(rhs._ptr) {}

        inline base_iterator& operator+=(difference_type rhs) { _ptr += rhs; return *this; }
        inline base_iterator& operator-=(difference_type rhs) { _ptr -= rhs; return *this; }
        inline base_iterator& operator++() { ++_ptr; return *this; }
        inline base_iterator& operator--() { --_ptr; return *this; }
        inline base_iterator operator++(int) const { base_iterator tmp(*this); ++_ptr; return tmp; }
        inline base_iterator operator--(int) const { base_iterator tmp(*this); --_ptr; return tmp; }
        inline difference_type operator-(const base_iterator& rhs) const { return base_iterator(_ptr - rhs._ptr); }
        inline base_iterator operator+(difference_type rhs) const { return base_iterator(_ptr + rhs); }
        inline base_iterator operator-(difference_type rhs) const { return base_iterator(_ptr - rhs); }

        friend inline base_iterator operator+(difference_type lhs, const base_iterator& rhs) { return base_iterator(lhs + rhs._ptr); }
        friend inline base_iterator operator-(difference_type lhs, const base_iterator& rhs) { return base_iterator(lhs - rhs._ptr); }

        inline bool operator==(const base_iterator& rhs) const { return _ptr == rhs._ptr; }
        inline bool operator!=(const base_iterator& rhs) const { return _ptr != rhs._ptr; }
        inline bool operator>(const base_iterator& rhs) const { return _ptr > rhs._ptr; }
        inline bool operator<(const base_iterator& rhs) const { return _ptr < rhs._ptr; }
        inline bool operator>=(const base_iterator& rhs) const { return _ptr >= rhs._ptr; }
        inline bool operator<=(const base_iterator& rhs) const { return _ptr <= rhs._ptr; }
    };

    /*
       class EntityStack::iterator : public base_iterator<typename container::iterator> {
       public:
       using iterator_category = std::random_access_iterator_tag;
       using difference_type = typename base_iterator<typename container::iterator>::difference_type;
       using reference = data_type&;
       using pointer = data_type*;
       using base_iterator<typename container::iterator>::base_iterator;
       inline reference operator[](difference_type rhs) const { return this->_ptr[rhs].first; }
       inline reference operator*()  { return this->_ptr->first; }
       inline pointer operator->() { return &(this->_ptr->first); }
       };
       */

    class EntityStack::const_iterator : public base_iterator<typename container::const_iterator> {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = typename base_iterator<typename container::const_iterator>::difference_type;
        using reference = const data_type&;
        using pointer = const data_type*;
        using base_iterator<typename container::const_iterator>::base_iterator;
        //inline reference operator[](difference_type rhs) const { return this->_ptr[rhs].first; }
        inline reference operator*() const { return this->_ptr->first; }
        inline pointer operator->() const { return &(this->_ptr->first); }
    };
}

#endif
