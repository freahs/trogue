#ifndef TROGUE_SHADOWCAST_H
#define TROGUE_SHADOWCAST_H

#include <vector>
#include <unordered_set>

#define PI 3.14159265

namespace trogue {

    class ShadowCast {
    private:

        class Node {
        private:
            std::vector<Node*>  m_children;
            bool                m_visible;
        public:
            Node();
            void visible(bool);
            bool visible() const;
            void set(bool);
            void add(Node*);
            bool contains(Node*) const;
        };

        int     m_center;
        int     m_height;
        int     m_width;
        Node*   m_arr;

        void scan(int, int);
        Node* get(int, int) const;
        float slope(float, float, float, float);

    public:
        ShadowCast(int);
        ~ShadowCast();

        void visible(int, int, bool);
        bool visible(int, int) const;
        void reset(int);
        void print();
    };

}

#endif
