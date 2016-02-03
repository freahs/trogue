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
#include <sstream>
#include <iomanip>
#include "../inc/display.hpp"
#include "../inc/format.hpp"
#include "../inc/scene.hpp"
#include "../inc/tile.hpp"
#include "../inc/tilestack.hpp"
#include "../inc/input.hpp"

void makeScene(trogue::Scene& scene) {
    scene.empty(trogue::Tile(trogue::Format(false, false, false, 234, 234), " ", 1));
    scene.emptyAlternative(trogue::Tile(trogue::Format(false, false, false, 238, 234), ".", 1));
    for (size_t y = 0; y < scene.height(); ++y){
        for (size_t x = 0; x < scene.width(); ++x) {
            std::stringstream ss;
            ss << std::hex << (x*y % 16);
            scene.add(x, y, trogue::Tile(trogue::Format(false, false, false, x, y), ss.str(), 1));
        }
    }
}

int main() {
    struct winsize ws;
    ioctl(0, TIOCGWINSZ, &ws);
    trogue::Scene scene(16, 16);
    makeScene(scene);
    trogue::Display * display = trogue::Display::instance();
    int width = 20;
    int height = 10;

    int x = 1;
    int y = 1;

    while (x < (int) scene.width() - 1) {
        display->draw(++x, y, width, height, scene);
    }

    while (y < (int) scene.height() - 1) {
        display->draw(x, ++y, width, height, scene);
    }

    while (x > 0) {
        display->draw(--x, y, width, height, scene);
    }

    while (y > 0) {
        display->draw(x, --y, width, height, scene);
    }
}

