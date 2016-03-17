#include "../inc/shadowcast.hpp"

#include "../inc/format.hpp"

#include <algorithm>
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

    float ShadowCast::slope(float y1, float x1, float y2, float x2) {
        return (x1 - x2) / (y1 - y2);
    }

    void ShadowCast::scan(int row, int col) {

        // temp child container for uniqueness
        std::unordered_set<Node*> temp_children;

        // The node which shadow are calculated
        Node* node = get(row, col);

        // default ranges for iteration
        int y_start = 0, y_stop = m_height;
        int x_start = 0, x_stop = m_width;

        float row_mod = 0, col_mod = 0;

        // Translate to 0-centered coordinates to identify region of the node
        int trans_y = row - m_center;
        int trans_x = col - m_center;

        // set parameters according to region of the node
        if (std::abs(trans_x) <= std::abs(trans_y)) {
            if (trans_y < 0)    { y_stop = row;  col_mod = 0.5f; }  // top
            else                { y_start = row; col_mod = -0.5f; } // bottom
        } else if (trans_x < 0) { x_stop = col;  row_mod = 0.5f; } // left
        else                    { x_start = col; row_mod = 0.5f; }  // right

        // calculate slope from center. the slopes (rays) are modified according to
        // their region.  
        float slope_a = slope(m_center, m_center, row + row_mod, col + col_mod);
        float slope_b = slope(m_center, m_center, row + row_mod*(-1.0f), col + col_mod*(-1.0f));

        // iterate over the nodes that are behind the node relative from the center
        for (int y = y_start; y >= 0 && y < y_stop; ++y) {
            for (int x = x_start; x >= 0 && x < x_stop; ++x) {
                //if (temp_children.find(get(y,x)) != temp_children.end()) break;

               float center_slope = slope(m_center, m_center, y, x);

                // if the current tile are inside the two slopes (ie if its in the shadow of the node
                // beeing processed) add it to that nodes child nodes
                if (center_slope > slope_a && center_slope < slope_b) {
                    temp_children.insert(get(y,x));
                    //node->add(get(y,x));
                }
            }
        }

        for (auto n : temp_children) {
            node->add(n);
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
        for (int row = 0; row < m_width; ++row) {
            for (int col = 0; col < m_height; ++col) {
                scan(row, col);
            }
        }
        reset(length);
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
