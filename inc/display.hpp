#ifndef TROGUE_DISPLAY_H
#define TROGUE_DISPLAY_H

#include "scene.hpp"
#include "format.hpp"

#include <iostream>
#include <signal.h>
#include <sys/ioctl.h>

namespace trogue {

    class Display {
        private:
            static size_t   s_rows;
            static size_t   s_cols;
            static Display* s_display;

            Display() {
                signal(SIGWINCH, Display::resizeHandler);
                resizeHandler(0);
            }

            static void resizeHandler(int) {
                struct winsize ws;
                ioctl(0, TIOCGWINSZ, &ws);
                s_rows = ws.ws_row - 2;
                s_cols = ws.ws_col;
            }

        public:
            ~Display() {
                if (s_display != nullptr) {
                    delete s_display;
                }
            }

            static Display* instance(){
                if (s_display == nullptr) {
                    s_display = new Display();
                }
                return s_display;
            }

            void draw(int player_x, int player_y, int w, int h, const Scene& scene) {
                int half_screen_height = height()/2;
                int half_screen_width = width()/2;
                int half_height = h/2;
                int half_width = w/2;
                int y1 = half_screen_height - half_height;
                int y2 = half_screen_height + half_height;
                int x1 = half_screen_width - half_width;
                int x2 = half_screen_width + half_width;

                std::cout << "\033[?25l\033[0m\033[1;1H"; // clear format, hide cursor, postion 1,1
                const Format* prev_format = &Format::empty;
                Format::empty.apply(std::cout);

                for (int y = 0; y < (int) height(); ++y) {
                    for (int x = 0; x < (int) width(); ++x) {
                        if (x >= x1 && x <= x2 && y >= y1 && y <= y2) {
                            int scene_y = y - half_screen_height + player_y;
                            int scene_x = x - half_screen_width + player_x;
                            scene.get(scene_x, scene_y).print(std::cout, prev_format);
                        } else {
                            Tile::empty.print(std::cout, prev_format);
                        }
                    }
                    std::cout << std::endl;
                } 

                std::cout << "\033[?25h"; // unhide cursor
            }

            size_t width() const {
                return s_cols;
            }

            size_t height() const {
                return s_rows;
            }
    };

    Display* Display::s_display = nullptr;
    size_t Display::s_rows = 0;
    size_t Display::s_cols = 0;

}

#endif
