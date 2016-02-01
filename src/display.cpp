#include "../inc/display.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <signal.h>
#include <sys/ioctl.h>


namespace trogue {

    Display* Display::s_display = nullptr;
    size_t Display::s_rows = 0;
    size_t Display::s_cols = 0;

    Display::FormatNode::FormatNode()
        : m_bold(false), m_italic(false), m_underline(false), m_fg_color(-1), m_bg_color(-1) {
        }

    Display::FormatNode::FormatNode(bool bold, bool italic, bool underline, int fg_color, int bg_color)
        : m_bold(bold), m_italic(italic), m_underline(underline), m_fg_color(fg_color), m_bg_color(bg_color) {
        }

    std::string Display::FormatNode::boldf() const {
        if (bold()) return "\033[1m";
        return "\033[22m";
    }

    std::string Display::FormatNode::italicf() const {
        if (m_italic) return "\033[3m";
        return "\033[23m";
    }

    std::string Display::FormatNode::underlinef() const {
        if (m_underline) return "\033[4m";
        return "\033[24m";
    }

    std::string Display::FormatNode::fgcolorf() const {
        if (m_fg_color == -1) return "\033[39m";
        return "\033[38;5;" + std::to_string(m_fg_color) + "m";
    }

    std::string Display::FormatNode::bgcolorf() const {
        if (m_bg_color == -1) return "\033[49m";
        return "\033[48;5;" + std::to_string(m_bg_color) + "m";
    }

    void Display::FormatNode::format(std::ostream& ss) const {
        ss <<  boldf() << italicf() << underlinef() << fgcolorf() << bgcolorf();
    }

    void Display::FormatNode::format(std::ostream& ss, FormatNode& other) const {
        if (this->m_bold != other.m_bold) ss << boldf();
        if (this->m_italic != other.m_italic) ss << italicf();
        if (this->m_underline != other.m_underline) ss << underlinef();
        if (this->m_fg_color != other.m_fg_color) ss << fgcolorf();
        if (this->m_bg_color != other.m_bg_color) ss << bgcolorf();
    }

    Display::TextNode::TextNode()
        : FormatNode(), m_text(" ") {
        }

    Display::TextNode::TextNode(const std::string& text)
        : FormatNode(), m_text(text) {
        }

    Display::TextNode::TextNode(const FormatNode& format, const std::string& text) 
        : FormatNode(format), m_text(text) {
        }


    Display::Display() {
        signal(SIGWINCH, Display::resizeHandler);
        resizeHandler(0);
        m_arr = std::vector<std::vector<TextNode>>(s_rows);
        for (size_t i = 0; i < s_rows; ++i) {
            m_arr[i] = std::vector<TextNode>(s_cols);
        }
    }

    Display::~Display() {
        if (s_display != nullptr) {
            delete s_display;
        }
    }

    void Display::resizeHandler(int) {
        struct winsize ws;
        ioctl(0, TIOCGWINSZ, &ws);
        s_rows = ws.ws_row - 2;
        s_cols = ws.ws_col;
    }

    void Display::reset() {
        for (size_t y = 0; y < m_arr.size(); ++y) {
            for (size_t x = 0; x < m_arr.size(); ++x) {
                m_arr[y][x] = TextNode(); 
            }
        }
    }

    Display* Display::instance(){
        if (s_display == nullptr) {
            s_display = new Display();
        }
        return s_display;
    }

    void Display::color(int fg, int bg) {
        m_current_format.color(fg, bg);
    }

    void Display::format(uint8_t format) {
        m_current_format.bold(format & BOLD); 
        m_current_format.italic(format & ITALIC);
        m_current_format.underline(format & UNDERLINE);
    }

    void Display::put(size_t x, size_t y, std::string text) {
        m_arr[y][x] = TextNode(m_current_format, text);
    }

    void Display::draw() {
        FormatNode empty_node;
        FormatNode* prev_node = &empty_node; 
        std::cout << "\033[?25l\033[0m" << std::endl;
        for(size_t row = 0; row < m_arr.size(); ++row) {
            std::cout  << "\033[" << row + 1 << ";1H";
            for (size_t col = 0; col < m_arr[row].size(); ++col) {
                m_arr[row][col].format(std::cout, *prev_node);
                std::cout << m_arr[row][col].str();
                prev_node = &m_arr[row][col];
            }
            std::cout << std::endl;
        }
        std::cout << "\033[?25h";
    } 

    size_t Display::width() const {
        return s_cols;
    }

    size_t Display::height() const {
        return s_rows;
    }

}
