#define _XOPEN_SOURCE_EXTENDED
#include "../inc/components.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>
#include <iomanip>
#include "shadowcast.hpp"
#include "map.hpp"
#include "input.hpp"
#include "scene.hpp"
#include "tilesystem.hpp"
#include "movementsystem.hpp"
#include "playersystem.hpp"

typedef std::chrono::high_resolution_clock      Time;
typedef std::chrono::milliseconds               Ms;
typedef std::chrono::system_clock::time_point   TimePoint;

int main() {


    int width = 50;
    int height = 50;
    // trogue::AutomataMap map(size*2, size*2);
    // map.generate(0.4f);
    trogue::AutomataMapBuilder map_builder(height*2, width*2, 0.4f);
    for (int i = 0; i < 0; ++i) {
        map_builder.build(5, 1);
    }
    for (int i = 0; i < 3; ++i) {
        map_builder.build(5, -1);
    }

    trogue::Map map = map_builder.finalize(); 
    trogue::Scene scene(map, 4);

    tyra::World world;
    world.system().add<trogue::MovementSystem>(scene);
    world.system().add<trogue::PlayerSystem>(scene);
    world.system().add<trogue::TileSystem>(scene, *trogue::Display::instance(), height, width);


    for (int y = 0; y < map.height(); ++y) {
        for (int x = 0; x < map.width(); ++x) {
            tyra::EntityId id = world.entity().create();
            world.component().add<trogue::PositionComponent>(id, y, x);
            if (map.visible(y, x)){
                world.component().add<trogue::TileComponent>(id, ".", 250, 240, -1, -1, 0);
            } else {
                world.component().add<trogue::TileComponent>(id, "X", 250, 240, -1, -1, 0);
            }
        }
    }

    auto pid = world.entity().create();
    world.component().add<trogue::PositionComponent>(pid, scene.height()/2, scene.width()/2);
    world.component().add<trogue::TileComponent>(pid, "@", 3, 3, -1, -1, 1);
    world.component().add<trogue::SightComponent>(pid, 20);
    world.component().add<trogue::PlayerComponent>(pid);
    world.tag("PLAYER", pid);


    trogue::Input input;
    trogue::Key res = trogue::Key::NONE;

    world.start();
    while (res != trogue::Key::Q) {
        if (input.has()) {
            int vy = 0;
            int vx = 0;
            res = input.get();
            if      (res == trogue::Key::UP)    { vy = -1; }
            else if (res == trogue::Key::DOWN)  { vy = 1; }
            else if (res == trogue::Key::LEFT)  { vx = -1; }
            else if (res == trogue::Key::RIGHT) { vx = 1; }
            world.component().add<trogue::MovementComponent>(pid, vy, vx);
        }

        world.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    /*
    std::cout << "MAIN LOOP" << std::endl;
    while (res != trogue::Key::Q) {
        world.update();
        if (input.has()) {
            int vy = 0;
            int vx = 0;
            res = input.get();
            if (res == trogue::Key::UP && y > 0) { vy = -1; }
            else if (res == trogue::Key::DOWN && y < scene.height()) { vy = 1; }
            else if (res == trogue::Key::LEFT && x > 0) { vx = -1; }
            else if (res == trogue::Key::RIGHT && x < scene.width()) { vx = 1; }
            world.component().add<trogue::MovementComponent>(pid, vy, vx);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    */




}


