#define _XOPEN_SOURCE_EXTENDED
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <stdio.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "../inc/components.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include "../inc/display.hpp"

void sighandler(int sig) {
    struct winsize ws;
    int rv = ioctl(0, TIOCGWINSZ, &ws);
    printf("IOCTL: rv=%d rows=%d cols=%d\n", rv, ws.ws_row, ws.ws_col);
}
/*
   int main(void){
   setlocale (LC_ALL,"");
   initscr();          
   start_color();
   use_default_colors();

   trogue::DisplayComponent dc("∀", 1, 1);

   for (int fg = 0; fg < COLORS; ++fg) {
   for (int bg = 0; bg < COLORS; ++bg) {
   int pos = COLORS*bg + fg;
   init_pair(pos, fg, bg);
   }
   }

   for(int i = 0; i < COLORS; ++i) {
   char str[3];
   int pos = i * COLORS;
   attron(COLOR_PAIR(pos));
   sprintf(str, "%d ", pos);
   addstr(str);
   attroff(COLOR_PAIR(pos));
   }
   getch();
   endwin();       
   return 0;
   }
   */

void table(const char *title, const char *mode)
{
    int f, b;
    printf("\n\033[1m%s\033[m\n bg\t fg\n", title);
    for (b = 40; b <= 107; b++) {
        if (b == 48) b = 100;
        printf("%3d\t\033[%s%dm", b, mode, b);
        for (f = 30; f <= 97; f++) {
            if (f == 38) f = 90;
            printf("\033[%dm%3d ", f, f);
        }
        puts("\033[m");
    }
}

int main() {
    std::cout << "main_debug" << std::endl;
    trogue::Display* d = trogue::Display::instance();

    size_t width = d->width();
    size_t height = d->height();

    int fg = 0;
    for (size_t i = 0; i < 25; ++i) {
        d->color(-1, -1);
        for(size_t x = 0; x < width;  ++x){
            d->put(x, 0, "u");
            d->put(x, height - 1, "d");
        }
        for (size_t y = 0; y < height; ++y) {
            d->put(0, y, "l");
            d->put(width - 1, y, "r");
        }
        for (size_t x = 1; x < width - 1; ++x) {
            for (size_t y = 1; y < height - 1; ++y) {
                fg = (fg + 1) % 256;
                if (fg % 2 == 1) {
                    d->format(trogue::Display::BOLD);
                    d->color(-1, -1);
                } else {
                    d->format(trogue::Display::ITALIC);
                    d->color(-1, -1);
                }
                int bg = 255 - fg;
                d->put(x, y, "@");
            }
        }
        d->draw();
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

}
