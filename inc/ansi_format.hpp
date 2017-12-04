/*
 * Copyright 2016 Fredrik Åhs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANSI_FORMAT_H
#define ANSI_FORMAT_H

#include <ostream>

namespace format {

    template<int ON, int OFF> class binary {
        bool m_status;
    public:
        binary(bool status) : m_status(status) { }
        std::ostream& operator()(std::ostream& os) const {
            os << "\033[";
            if (m_status) { os << ON; }
            else          { os << OFF; }
            return os << "m";
        }
        friend inline std::ostream& operator<<(std::ostream& os, binary<ON, OFF> b) {
            return b(os);
        }
    };

    template <int CODE> class color_8 {
        uint8_t m_color;
    public:
        color_8(uint8_t color) : m_color(color) { }
        std::ostream& operator()(std::ostream& os) const {
            return os << "\033["<< CODE << ";5;" << static_cast<int>(m_color) << "m";
        }
        friend inline std::ostream& operator<<(std::ostream& os, const color_8<CODE> color) {
            return color(os);
        }
    };

    template <int CODE> class color_24 {
        uint8_t m_r, m_g, m_b;
    public:
        color_24(uint8_t r, uint8_t g, uint8_t b) : m_r(r), m_g(g), m_b(b) { }
        std::ostream& operator()(std::ostream& os) const {
            os << "\033[" << CODE << ";2;" << static_cast<int>(m_r) << ";" << static_cast<int>(m_g) << ";" << static_cast<int>(m_b) << "m";
            return os;
        }
        friend inline std::ostream& operator<<(std::ostream& os, const color_24<CODE> color) {
            return color(os);
        }
    };

    template <int CODE> struct color_default {
        std::ostream& operator()(std::ostream& os) const {
            return os << "\033[" << CODE << "m";
        }
        friend inline std::ostream& operator<<(std::ostream& os, const color_default<CODE> color) {
            return color(os);
        }
    };

    class pos {
        int m_row, m_col;
    public:
        pos(int row, int col) : m_row(row), m_col(col) { }
        std::ostream& operator()(std::ostream& os) const {
            return os << "\033[" << m_row << ";" << m_col <<  "H";
        }
        friend inline std::ostream& operator<<(std::ostream& os, pos p) {
            return p(os);
        }
    };

    class rpos {
        int m_row, m_col;
    public:
        rpos(int row, int col) : m_row(row), m_col(col) { }
        std::ostream& operator()(std::ostream& os) const {
            if (m_row < 0) { os << "\033[" << -m_row << "A"; }
            if (m_row > 0) { os << "\033[" << m_row << "B"; }
            if (m_col < 0) { os << "\033[" << -m_col << "D"; }
            if (m_col > 0) { os << "\033[" << m_col << "C"; }
            return os;
        }
        friend inline std::ostream& operator<<(std::ostream& os, rpos rp) {
            return rp(os);
        }

    };

    class hide {
        bool m_hidden;
    public:
        hide(bool hidden) : m_hidden(hidden) { }
        std::ostream& operator()(std::ostream& os) const {
            if (m_hidden) { return os << "\033[?25l"; }
            else          { return os << "\033[?25h"; }
        }
        friend inline std::ostream& operator<<(std::ostream& os, hide h) {
            return h(os);
        }
    };

    // Common formats
    typedef binary<1, 22> bold;
    typedef binary<3, 23> italic;
    typedef binary<4, 24> underline;

    // Foreground and background color
    inline color_8<38> fg(uint8_t c) { return color_8<38>(c); }
    inline color_24<38> fg(uint8_t r, uint8_t g, uint8_t b) { return color_24<38>(r, g, b); }
    using fg_default = color_default<39>;
    inline color_8<48> bg(uint8_t c) { return color_8<48>(c); }
    inline color_24<48> bg(uint8_t r, uint8_t g, uint8_t b) { return color_24<48>(r, g, b); }
    using bg_default = color_default<49>;

    // clear funtion
    inline std::ostream& clear(std::ostream& os) {
        os << "\033[0m";
        return os;
    }

}

#endif
