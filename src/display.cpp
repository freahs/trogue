#include "../inc/display.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include <signal.h>
#include <sys/ioctl.h>

#define ANSI_CLEAR "\033[0m"

namespace trogue {

    Display* Display::s_display = nullptr;
    size_t Display::s_rows = 0;
    size_t Display::s_cols = 0;

    Display::Display() {
        signal(SIGWINCH, Display::resizeHandler);
        resizeHandler(0);
        m_arr = std::vector<std::vector<std::string>>(s_rows, std::vector<std::string>(s_cols, "\033[39m\033[49m "));
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
                m_arr[y][x] = "\033[39m\033[49m "; 
            }
        }
    }

    Display* Display::instance(){
        if (s_display == nullptr) {
            s_display = new Display();
        }
        return s_display;
    }
    
    void Display::put(size_t x, size_t y, std::string str) {
        put(x, y, "\033[39m", "\033[49m", str);
    }

    void Display::put(size_t x, size_t y, uint8_t fg, uint8_t bg, std::string text) {
        std::string bg_str = "\033[48;5;" + std::to_string(bg) + "m";
        std::string fg_str = "\033[38;5;" + std::to_string(fg) + "m";
        put(x, y, fg_str, bg_str, text);
    }

    void Display::put(size_t x, size_t y, std::string fg_str, std::string bg_str, std::string text) {
        m_arr[y][x] = fg_str + bg_str + text;
    }


    void Display::draw() {
        std::cout << "\033[?25l\033[0m" << std::endl;
        for(size_t row = 0; row < s_rows; ++row) {
            std::cout  << "\033[" << row + 1 << ";1H";
            for (size_t col = 0; col < s_cols; ++col) {
                std::cout << m_arr[row][col];
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
