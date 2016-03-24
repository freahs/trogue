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

    // A binary (on/off) functor that applies an ANSI format to an ostream.
    template<int ON, int OFF> class Format {
    private:
        static bool s_current_status;
        bool m_status;

    public:
        Format(bool status) : m_status(status) { }

        // When operator() is called with an ostream, apply format if
        // not already active
        std::ostream& operator()(std::ostream& os) const {
            if (m_status != s_current_status) {
                os << "\033[";
                if (m_status) { os << ON; }
                else { os << OFF; }
                os << "m";
            }
            s_current_status = m_status;
            return os;
        }
    };

    // Functor that applies a color to an ostream
    template<int ON, int OFF> class Color {
    private:
        static int s_current_color;
        int m_color;

    public:
        Color(int color) : m_color(color) { }

        std::ostream& operator()(std::ostream& os) const {
            if (m_color != s_current_color) {
                os << "\033[";
                if (m_color  == -1) { os << OFF; }
                else { os << ON << ";5;" << m_color; }
                os << "m";
            }
            s_current_color = m_color;
            return os;
        }
    };

    // Functor that positions the cursor in an ostream
    class pos {
    private:
        int m_row;
        int m_col;

    public:
        pos(int row, int col) : m_row(row), m_col(col) { }

        std::ostream& operator()(std::ostream& os) const {
            os << "\033[" << m_row << ";" << m_col <<  "H";
            return os;
        }
    };

    class hide {
    private:
        bool            m_hide;
    public:
        hide(bool hide) : m_hide(hide) { }

        std::ostream& operator()(std::ostream& os) const {
            if (m_hide) {
                os << "\033[?25l";
            } else {
                os << "\033[?25h";
            }
            return os;
        }
    };

    // Initialize format as inactive
    template<int ON, int OFF>
    bool Format<ON, OFF>::s_current_status = false;

    // Make operator<< call operator() on the functor
    template<int ON, int OFF>
    inline std::ostream& operator<<(std::ostream& os, Format<ON, OFF> format) {
        return format(os);
    }

    // -1 represents default terminal color
    template<int ON, int OFF>
    int Color<ON, OFF>::s_current_color = -1;

    template<int ON, int OFF>
    inline std::ostream& operator<<(std::ostream& os, Color<ON, OFF> color) {
        return color(os);
    }

    // Common formats
    typedef Format<1, 22> bold;
    typedef Format<3, 23> italic;
    typedef Format<4, 24> underline;

    // Foreground and background color
    typedef Color<38, 39> fg;
    typedef Color<48, 49> bg;

    inline std::ostream& operator<<(std::ostream& os, pos p) {
        return p(os);
    }


    inline std::ostream& operator<<(std::ostream& os, hide h) {
       return h(os);
    } 

    // clear funtion
    inline std::ostream& clear(std::ostream& os) {
        os << "\033[0m";
        return os;
    }

}

#endif
