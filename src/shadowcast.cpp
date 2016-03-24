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

    // when calculating shadows in the first and eight quadrant in the picture above,
    // rays cast from the center should target the left top and bottom corner of a grid
    // cell if the target cell is on a straight axis (type a in the picture) or the top
    // right and bottom left corner if its in the first quadrant and the top left, bottom
    // right if its in the eight quadrant.

    const float ShadowCast::Slope::mods[3][4] = {
        { 0.5f,  0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f,  0.5f}
    };

    ShadowCast::Slope::Slope(int y, int x) {
        const float* mod;
        if      (y < 0)     { mod = mods[0]; }
        else if (y == 0)    { mod = mods[1]; }
        else                { mod = mods[2]; }
        top =    (static_cast<float>(y) + mod[0])/(static_cast<float>(x) + mod[1]);
        center =  static_cast<float>(y)          / static_cast<float>(x);
        bottom = (static_cast<float>(y) + mod[2])/(static_cast<float>(x) + mod[3]);
    }

    void ShadowCast::Node::add(Node* node, bool full) {
        if (node != this) {
            if (full)   { m_full_children.push_back(node); }
            else        { m_part_children.push_back(node); }
        }
    }

    void ShadowCast::calculate(int oy, int ox) {

        // rays from origin to obstacle
        Slope o_slope(oy, ox);

        for (int tx = ox; tx <= m_size; ++tx) {

            int y_start = std::ceil(o_slope.bottom*tx);
            int y_stop  = std::floor(o_slope.top*tx);

            for (int ty = y_start; ty <= y_stop && ty <= m_size; ++ty) {

                // rays from origin to cell to check if shaded
                Slope t_slope(ty, tx);

                if (t_slope.center < o_slope.top && t_slope.center > o_slope.bottom) {

                    bool full = t_slope.top < o_slope.top && t_slope.bottom >o_slope.bottom;

                    // shared by all (type a, b and c)
                    node( oy,  ox)->add(node( ty,  tx), full);
                    node( oy, -ox)->add(node( ty, -tx), full);

                    // shared by all not on diagonal axis (type a and c)
                    if (oy != ox) {
                        node( ox,  oy)->add(node( tx,  ty), full);
                        node(-ox,  oy)->add(node(-tx,  ty), full);
                    }

                    // shared by all not on straight axis (type b and c)
                    if (oy != 0) {
                        node(-oy,  ox)->add(node(-ty,  tx), full);
                        node(-oy, -ox)->add(node(-ty, -tx), full);
                    }

                    // shared by all not on any axis (type c)
                    if (oy != ox && oy != 0) {
                        node( ox, -oy)->add(node( tx, -ty), full);
                        node(-ox, -oy)->add(node(-tx, -ty), full);
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


    ShadowCast::ShadowCast(int length)
        : m_size(length),
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
        if (!n->m_full) {
            for (Node* c : n->m_full_children) {
                c->m_full = true;
                c->m_part = false;
            }
            for (Node* c : n->m_part_children) {
                if (!c->m_full) {
                    c->m_part = true;
                }
            }
        }
    }

    bool ShadowCast::visible(int y, int x) const {
        return !node(y, x)->m_full && !node(y, x)->m_part;
    }

    void ShadowCast::reset(int range) {
        for (int y = -m_size; y <= m_size; ++y) {
            for (int x = -m_size; x <= m_size; ++x) {
                Node* n = node(y, x);
                n->m_part = false;
                if (y*y + x*x >= range*range) {
                    n->m_full = true;
                } else {
                    n->m_full = false;
                }
            }
        }
    }

    int ShadowCast::size() const {
        return m_size;
    }

    void ShadowCast::print() const {
        for (int y = -m_size; y <= m_size; ++y) {
            for (int x = -m_size; x <= m_size; ++x) {
                Node* n = node(y, x);
                if (y == 0 && x == 0) {
                    std::cout << "@";
                } else if (n->m_full) {
                    std::cout << "o";
                } else if (n->m_part) {
                    std::cout << "#";
                } else {
                    std::cout << ".";
                }
            }
            std::cout << std::endl;
        }
    }

}
