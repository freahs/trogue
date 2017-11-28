#include "../inc/xterm_display.hpp"

#include "../inc/ansi_format.hpp"

#include <iostream>
#include <signal.h>
#include <sys/ioctl.h>
#include <vector>

namespace trogue {


    Display::Display() : m_rows(0), m_cols(0), m_was_resized(true){ }

    void Display::resizeHandler(int) {
        struct winsize ws;
        ioctl(0, TIOCGWINSZ, &ws);
        int terminal_rows = ws.ws_row;
        int terminal_cols = ws.ws_col;
        instance()->resize(terminal_rows, terminal_cols);
    }

    Display::Data& Display::data(int row, int col) {
        return m_data[row*m_cols + col];
    }

    Display::~Display() {
        if (Display::s_display != nullptr) {
            delete s_display;
        }
    }

    Display* Display::instance(){
        if (s_display == nullptr) {
            s_display = new Display();
            signal(SIGWINCH, Display::resizeHandler);
            resizeHandler(0);
        }
        return s_display;
    }

    void Display::resize(int rows, int cols) {
        size_t delta_y = (m_rows - rows)/2;
        size_t delta_x = (m_cols - cols)/2;
        auto data_tmp = std::vector<Data>(rows*cols);

        for (int y = 0; y < rows; ++y) {
            // If the screen is now larger, some tiles will be outside the old matrix. these are initialized as
            // empty and in need of update. Since these tiles are outside the old matrix, the inner loop can be skipped
            int old_y = y + delta_y;
            if (old_y >= 0 && old_y < m_rows) {
                for (int x = 0; x < cols; ++x) {
                    int old_x = x + delta_x;
                    if (old_x >= 0 && old_x < m_cols) {
                        data_tmp[y*cols + x] = data(old_y, old_x);
                        data_tmp[y*cols + x].m_needs_update = true;
                    }
                }
            }
        }
        m_data = data_tmp;
        m_rows = rows;
        m_cols = cols;
    }

    void Display::set(size_t row, size_t col, const std::string& symbol, int color, int bg_color) {
        Data& d = data(row, col);
        if (d.m_symbol != symbol || d.m_color != color || d.m_bg_color != bg_color) {
            data(row, col) = {symbol, color, bg_color, true};
        }
    }

    void Display::draw(std::ostream& os) {
        bool need_positioning = true;

        os << format::hide(true);
        for (int y = 0; y < height(); ++y) {
            for (int x = 0; x < width(); ++x) {
                Data& d = data(y,x);
                if (d.m_needs_update) {
                    if (need_positioning) {
                        os << format::pos(y + 1, x + 1);
                    }

                    // FG COLOR
                    if (d.m_color == -1) {
                        os << format::fg_default();
                    } else {
                        os << format::fg((d.m_color >> 16 & 0xFF), (d.m_color >> 8 & 0xFF), (d.m_color & 0xFF));
                    }

                    //BG COLOR
                    if (d.m_bg_color == -1) {
                        os << format::bg_default();
                    } else {
                        os << format::bg((d.m_bg_color >> 16 & 0xFF), (d.m_bg_color >> 8 & 0xFF), (d.m_bg_color & 0xFF));
                    }
                    os << d.m_symbol;
                    need_positioning = false;
                } else {
                    need_positioning = true;
                }
            }
            os << std::endl;
        }
        os << format::hide(false);
    }

    int Display::width() const {
        return m_cols;
    }

    int Display::height() const {
        return m_rows - 1;
    }

    Display* Display::s_display = nullptr;

}

