#ifndef TROGUE_SHADOWCAST_H
#define TROGUE_SHADOWCAST_H

#include <vector>

namespace trogue {

    class ShadowCast {
    private:

        struct Slope {
            static const float mods[3][4];
            float top;
            float center;
            float bottom;
            Slope(int y, int x);
        };

        struct Node {
            std::vector<Node*>  m_part_children;
            std::vector<Node*>  m_full_children;
            bool                m_full = false;
            bool                m_part = false;
            void add(Node*, bool);
        };

        int     m_size;
        Node*   m_arr;

        Node* node(int, int) const;
        void calculate(int, int);

    public:
        ShadowCast(int);
        ~ShadowCast();

        void set(int, int);
        bool visible(int, int) const;
        void reset(int);
        int size() const;

        void print() const;
    };

}

#endif
