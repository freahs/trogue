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

    const float ShadowCast::Slope::mods[3][4] = {
        {-0.5f, -0.5f, 0.5f,  0.5f},
        {-0.5f, -0.5f, 0.5f, -0.5f},
        {-0.5f,  0.5f, 0.5f, -0.5f}
    };

    ShadowCast::Slope::Slope(int y, int x) {
        const float* mod;
        if (y < 0) { mod = mods[0]; }
        else if (y == 0) { mod = mods[1]; }
        else { mod = mods[2]; }
        top = (static_cast<float>(y) + mod[0])/(static_cast<float>(x) + mod[1]);
        center = static_cast<float>(y)/static_cast<float>(x);
        bottom = (static_cast<float>(y) + mod[2])/(static_cast<float>(x) + mod[3]);
    }

    void ShadowCast::Node::add(Node* node) {
        if (node == this) { return; }
        m_children.push_back(node);
    }

    void ShadowCast::calculate(int oy, int ox) {

        // when calculating shadows in the first quadrant in the picture above, rays
        // cast from the center should target the left top and bottom corner of a grid
        // cell if the target cell if the target cell is on a straight axis (type a in 
        // the picture) or the top right and bottom left corner if not.
        static float mods[2][4] = {{0.5f, -0.5f, -0.5f, -0.5f},{0.5f, -0.5f, -0.5f, 0.5f}};
        float* mod;

        if (oy == 0) {
            mod = mods[0];
        } else {
            mod = mods[1];
        }

        float top_slope =    (static_cast<float>(oy) + mod[0])/(static_cast<float>(ox) + mod[1]);
        float bottom_slope = (static_cast<float>(oy) + mod[2])/(static_cast<float>(ox) + mod[3]);

        for (int x = ox; x <= m_size; ++x) {
            int y_start = std::ceil(bottom_slope*x);
            int y_stop  = std::floor(top_slope*x);
            for (int y = y_start; y <= y_stop && y <= m_size; ++y) {

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
                    node( oy,  ox)->add(node( y,  x));
                    node( oy, -ox)->add(node( y, -x));

                    // shared by all not on diagonal axis
                    if (oy != ox) {
                        node( ox,  oy)->add(node( x,  y));
                        node(-ox,  oy)->add(node(-x,  y));
                    }

                    // shared by all not on straight axis
                    if (oy != 0) {
                        node(-oy,  ox)->add(node(-y,  x));
                        node(-oy, -ox)->add(node(-y, -x));
                    }

                    // shared by all not on any axis
                    if (oy != ox && oy != 0) {
                        node( ox, -oy)->add(node( x, -y));
                        node(-ox, -oy)->add(node(-x, -y));
                    }
                }
            }
        }
    }

    ShadowCast::Node* ShadowCast::node(int y, int x) const {
        int row = m_size + y;
        int col = m_size + x;
        return &m_arr[row*(2*m_size + 1) + col];
    }

    ShadowCast::ShadowCast(int length) : ShadowCast(length, true) { }

    ShadowCast::ShadowCast(int length, bool strict)
        : m_size(length),
        m_strict(strict),
        m_arr(new Node[(2*m_size + 1)*(2*m_size + 1)]) {

            // calculate shadows for all nodes in the first quardrant. (which will populate
            // the whole matrix)
            for (int x = 1; x <= m_size; ++x) {
                for (int y = 0; y <= x; ++y) {
                    calculate(y, x);
                }
            }

        }

    ShadowCast::~ShadowCast() {
        delete [] m_arr;
    }

    void ShadowCast::set(int y, int x) {
        Node * n = node(y, x);
        if (!n->m_obstacle) {
            n->m_obstacle = true;
            n->m_shaded = true;
            for (Node* c : n->m_children) {
                c->m_shaded = true;
            }
        }
    }

    bool ShadowCast::get(int y, int x) const {
        return !node(y, x)->m_shaded;
    }

    void ShadowCast::reset(int range) {
        for (int y = -m_size; y <= m_size; ++y) {
            for (int x = -m_size; x <= m_size; ++x) {
                Node* n = node(y, x);
                n->m_obstacle = false;
                if (y*y + x*x >= range*range) {
                    n->m_shaded = true;
                } else {
                    n->m_shaded = false;
                }
            }
        }
    }

    void ShadowCast::print() {
        for (int y = -m_size; y <= m_size; ++y) {
            for (int x = -m_size; x <= m_size; ++x) {
                Node* n = node(y, x);
                if (y == 0 && x == 0) {
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
