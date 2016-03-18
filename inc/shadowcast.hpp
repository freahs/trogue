#ifndef TROGUE_SHADOWCAST_H
#define TROGUE_SHADOWCAST_H

#include <vector>

namespace trogue {

    class ShadowCast {
    private:

        struct Node {
            std::vector<Node*>  m_children;
            bool                m_obstacle = false;
            bool                m_shaded = false;
            void add(Node*);
        };

        int     m_center;
        bool    m_strict;
        Node*   m_arr;

        Node* node(int, int) const;
        void calculate(int, int);

    public:
        ShadowCast(int);
        ShadowCast(int, bool);
        ~ShadowCast();

        void set(int, int);
        bool get(int, int) const;
        void reset(int);
        void print();
    };

}

#endif
