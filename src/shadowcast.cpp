#include "../inc/shadowcast.hpp"

#include <cmath>
#include <vector>
#include <iostream>

/*

   -x, -y        x, -y
   \66666666|77777777/
   5\.......|......./8
   5.\......a....../.8
   5..\...c.|.c.../..8
   5...\....|..../...8
   5....b...|...b....8
   5.....\..|../.....8
   5..c...\.|./...c..8
   5.......\|/.......8
   --a------@------a--
   4......./|\.......1
   4..c.../.|.\...c..1
   4...../..|..\.....1
   4....b...|...b....1
   4.../....|....\...1
   4../...c.|.c...\..1
   4./......a......\.1
   4/.......|.......\1
   /33333333|22222222\
   -x, y          x, y

   Shadows are precalculated by calculating all shadows for the first quadrant
   and using symmetries to populate the other seven quadrants.

*/

namespace trogue {

    void ShadowCast::Node::add(Node* node) {
        if (node == this) { return; }
        m_children.push_back(node);
    }

    void ShadowCast::calculate(int row, int col) {

        // when calculating shadows in the first quadrant in the picture above, rays
        // cast from the center should target the left top and bottom corner of a grid
        // cell if the target cell if the target cell is on a straight axis (type a in 
        // the picture) or the top right and bottom left corner if not.
        static float mods[2][4] = {{0.5f, -0.5f, -0.5f, -0.5f},{0.5f, -0.5f, -0.5f, 0.5f}};
        float* mod;

        int trans_y = row - m_center;
        int trans_x = col - m_center;

        if (trans_y == 0) {
            mod = mods[0];
        } else {
            mod = mods[1];
        }

        float top_slope =    (static_cast<float>(trans_y) + mod[0])/(static_cast<float>(trans_x) + mod[1]);
        float bottom_slope = (static_cast<float>(trans_y) + mod[2])/(static_cast<float>(trans_x) + mod[3]);

        bool type_a = trans_y == 0;         // obstacle located on horizontal or vertical axis
        bool type_b = trans_y == trans_x;   // obstacle located on diagonal axis


        for (int x = trans_x; x <= m_center; ++x) {
            int y_start = std::ceil(bottom_slope*x);
            int y_stop  = std::floor(top_slope*x);
            for (int y = y_start; y <= y_stop && y <= m_center; ++y) {

                // calculate slope from center to the center of the current coordinate
                float slope = static_cast<float>(y)/static_cast<float>(x);

                // check if coordinate in the shadow of the current node depending on settings
                bool in_range;
                if (m_strict) {
                    in_range = slope < top_slope && slope > bottom_slope;
                } else {
                    in_range = slope <= top_slope && slope >= bottom_slope;
                } 

                if (in_range) {

                    // shared by all
                    node(m_center + trans_y, m_center + trans_x)->add(node(m_center + y, m_center + x));
                    node(m_center + trans_y, m_center - trans_x)->add(node(m_center + y, m_center - x));

                    // shared by all not on diagonal axis
                    if (!type_b) {
                        node(m_center + trans_x, m_center + trans_y)->add(node(m_center + x, m_center + y));
                        node(m_center - trans_x, m_center + trans_y)->add(node(m_center - x, m_center + y));
                    }

                    // shared by all not on straight axis
                    if (!type_a) {
                        node(m_center - trans_y, m_center + trans_x)->add(node(m_center - y, m_center + x));
                        node(m_center - trans_y, m_center - trans_x)->add(node(m_center - y, m_center - x));
                    }

                    // shared by all not on any axis
                    if (!type_a && !type_b) {
                        node(m_center + trans_x, m_center - trans_y)->add(node(m_center + x, m_center - y));
                        node(m_center - trans_x, m_center - trans_y)->add(node(m_center - x, m_center - y));
                    }
                }
            }
        }
    }

    ShadowCast::Node* ShadowCast::node(int row, int col) const {
        return &m_arr[row*(2*m_center + 1) + col];
    }

    ShadowCast::ShadowCast(int length) : ShadowCast(length, true) { }

    ShadowCast::ShadowCast(int length, bool strict)
        : m_center(length),
        m_strict(strict),
        m_arr(new Node[(2*length + 1)*(2*length + 1)]) {

            // calculate shadows for all nodes in the first quardrant. (which will populate
            // the whole matrix)
            for (int col = m_center + 1; col <= m_center*2; ++col) {
                for (int row = m_center; row <= col || row == 0; ++row) {
                    calculate(row, col);
                }
            }

        }

    ShadowCast::~ShadowCast() {
        delete [] m_arr;
    }

    void ShadowCast::set(int y, int x) {
        int row = y + m_center;
        int col = x + m_center;
        Node * n = node(row, col);
        if (!(n->m_shaded)) {
            n->m_obstacle = true;
            n->m_shaded = true;
            for (Node* c : n->m_children) {
                c->m_shaded = true;
            }
        }
    }

    bool ShadowCast::get(int y, int x) const {
        int row = y + m_center;
        int col = x + m_center;
        return !(node(row, col)->m_shaded);
    }

    void ShadowCast::reset(int range) {
        for (int y = 0; y < 2*m_center + 1; ++y) {
            for (int x = 0; x < 2*m_center + 1; ++x) {
                Node* n = node(y, x);
                int trans_y = y - m_center;
                int trans_x = x - m_center;
                n->m_obstacle = false;

                if (trans_x*trans_x + trans_y*trans_y >= range*range) {
                    n->m_shaded = true;
                } else {
                    n->m_shaded = false;
                }
            }
        }
    }

    void ShadowCast::print() {
        for (int y = 0; y < 2*m_center + 1; ++y) {
            for (int x = 0; x < 2*m_center + 1; ++x) {
                Node* n = node(y, x);
                if (y == m_center && x == m_center) {
                    std::cout << "@";
                } else if (n->m_obstacle) {
                    std::cout << "o";
                } else if (n->m_shaded) {
                    std::cout << "#";
                } else {
                    std::cout << ".";
                }
            }
            std::cout << std::endl;
        }
    }

}
