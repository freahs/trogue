#include "../inc/shadowcast.hpp"

#include "../inc/format.hpp"

#include <algorithm>
#include <cmath>
#include <unordered_set>
#include <vector>
#include <iostream>



namespace trogue {

    ShadowCast::Node::Node() : m_visible(true) {
    }

    void ShadowCast::Node::visible(bool visible) {
        m_visible = visible;
    }

    bool ShadowCast::Node::visible() const {
        return m_visible;
    }

    void ShadowCast::Node::set(bool b) {
        visible(b);
        for (Node* n : m_children) {
            n->visible(b);
        }
    }

    void ShadowCast::Node::add(Node* node) {
        if (node == this) { return; }
        m_children.push_back(node);
    }

    bool ShadowCast::Node::contains(Node* node) const {
        //return m_children.find(node) != m_children.end();
        return std::find(m_children.begin(), m_children.end(), node) != m_children.end();
    }

    /*

    \4444444444444/
    3\44444444444/1
    33\444444444/11
    333\4444444/111
    3333\44444/1111
    33333\444/11a11
    333333\4/111111
    3333333@1111111
    333333/2\111111
    33333/222\11111
    3333/22222\1111
    333/2222222\111
    33/222222222\11
    3/22222222222\1
    /2222222222222\

    3 = 7
    4 = 6

    5 = 4

    1   node( y, x) a = (-2, 5)
    2   node(-y, x) b = ( 2, 5)
    3   node(-x,-y) c = (-5, 2)
    4   node(-x, y) d = (-5,-2)
    5   node(-y,-x) e = ( 2,-5)
    6   node( y,-x) f = (-2,-5)
    7   node( x, y) g = ( 5,-2)
    8   node( x,-y) g = ( 5, 2)

    */

    void ShadowCast::scan2(int row, int col) {

        int trans_y = row - m_center;
        int trans_x = col - m_center;


        if (trans_y == 0) {
            std::cout << "trans y == 0" << std::endl;
            float top_slope =    (static_cast<float>(trans_y) + 0.5f)/(static_cast<float>(trans_x) - 0.5f);
            float bottom_slope = (static_cast<float>(trans_y) - 0.5f)/(static_cast<float>(trans_x) - 0.5f);
            for (int x = trans_x; x <= m_center; ++x) {
                for (int y = -x; y <= x; ++y) {
                    float slope = static_cast<float>(y)/static_cast<float>(x);
                    if (slope < top_slope && slope > bottom_slope) {
                    std::cout << top_slope << " " << slope << " " << bottom_slope << std::endl;
                        get(m_center + trans_y, m_center + trans_x)->add(get(m_center + y, m_center + x)); // Q1
                        get(m_center + trans_y, m_center - trans_x)->add(get(m_center + y, m_center - x)); // Q2
                        get(m_center + trans_x, m_center - trans_y)->add(get(m_center + x, m_center - y)); // Q3
                        get(m_center - trans_x, m_center - trans_y)->add(get(m_center - x, m_center - y)); // Q4
                    }
                }
            }
            return;
        }

        float top_slope =    (static_cast<float>(trans_y) + 0.5f)/(static_cast<float>(trans_x) - 0.5f);
        float bottom_slope = (static_cast<float>(trans_y) - 0.5f)/(static_cast<float>(trans_x) + 0.5f);

        for (int y = trans_y; y <= m_center; ++y) {
            for (int x = trans_x; x <= m_center; ++x) {
                float slope = static_cast<float>(y)/static_cast<float>(x);
                if (slope < top_slope && slope > bottom_slope) {
                    get(m_center + trans_y, m_center + trans_x)->add(get(m_center + y, m_center + x)); // Q1
                    get(m_center - trans_y, m_center + trans_x)->add(get(m_center - y, m_center + x)); // Q2
                    get(m_center + trans_y, m_center - trans_x)->add(get(m_center + y, m_center - x)); // Q3
                    get(m_center - trans_y, m_center - trans_x)->add(get(m_center - y, m_center - x)); // Q4
                    if (trans_y != trans_x) {
                        get(m_center + trans_x, m_center - trans_y)->add(get(m_center + x, m_center - y)); // Q5
                        get(m_center - trans_x, m_center - trans_y)->add(get(m_center - x, m_center - y)); // Q6
                        get(m_center - trans_x, m_center + trans_y)->add(get(m_center - x, m_center + y)); // Q7
                        get(m_center + trans_x, m_center + trans_y)->add(get(m_center + x, m_center + y)); // Q8
                    }

                }
            }
        }
    }

    ShadowCast::Node* ShadowCast::get(int row, int col) const {
        return &m_arr[row*m_height + col];
    }

    ShadowCast::ShadowCast(int length)
    : m_center(length),
    m_height(length*2 + 1),
    m_width(m_height),
    m_arr(new Node[m_height*m_width]) {
        for (int row = m_center + 1; row <= m_center*2; ++row) {
            for (int col = m_center + 1; col <= row || row == 0; ++col) {
                scan2(row, col);
            }
        }

        for (int col = m_center + 1; col <= m_center*2; ++col) {
            scan2(m_center, col);
        }
    }

    ShadowCast::~ShadowCast() {
        delete [] m_arr;
    }

    void ShadowCast::visible(int y, int x, bool visible) {
        int row = y + m_center;
        int col = x + m_center;
        if (get(row, col)->visible()) {
            get(row, col)->set(visible);
        }
    }

    bool ShadowCast::visible(int y, int x) const {
        int row = y + m_center;
        int col = x + m_center;
        return get(row, col)->visible();
    }

    void ShadowCast::reset(int range) {
        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                Node* n = get(y, x);
                int trans_y = y - m_center;
                int trans_x = x - m_center;

                if (trans_x*trans_x + trans_y*trans_y >= range*range) {
                    n->visible(false);
                } else {
                    n->visible(true);
                }
            }
        }
    }

    void ShadowCast::print() {
        for (int y = 0; y < m_height; ++y) {
            for (int x = 0; x < m_width; ++x) {
                if (y == m_center && x == m_center) {
                    std::cout << format::bg(3);
                } else if (get(y, x)->visible()) {
                    std::cout << format::bg(2);
                } else {
                    std::cout << format::bg(1);
                }
                std::cout << " ";
            }
            std::cout << format::bg(-1) << std::endl;
        }
    }

}
