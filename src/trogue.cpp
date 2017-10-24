#define _XOPEN_SOURCE_EXTENDED
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>
#include <iomanip>
#include "map.hpp"
#include "input.hpp"
#include "shadowcast.hpp"
#include "tilesystem.hpp"
#include "movementsystem.hpp"
#include "playersystem.hpp"
#include "aisystem.hpp"
#include "components.hpp"
#include "visibilitysystem.hpp"

typedef std::chrono::high_resolution_clock      Time;
typedef std::chrono::milliseconds               Ms;
typedef std::chrono::system_clock::time_point   TimePoint;

int main() {
    trogue::Map<bool> map(200, 200);
    for (auto y = 1; y < 200; ++y) {
        map[y][map.width()/2 + 1] = true;
    }
    trogue::Scene scene(map, 5);

    tyra::World world;
    world.system().add<trogue::AISystem>();
    world.system().add<trogue::MovementSystem>(map.height(), map.width());
    world.system().add<trogue::PlayerSystem>();
    world.system().add<trogue::VisibilitySystem>(map.height(), map.width());
    world.system().add<trogue::TileSystem>(*trogue::Display::instance(), map.height(), map.width());


    for (int y = 0; y < map.height(); ++y) {
        for (int x = 0; x < map.width(); ++x) {
            tyra::EntityId id = world.entity().create();
            world.component().add<trogue::PositionComponent>(id, y, x);
            if (map[y][x]){
                world.component().add<trogue::TileComponent>(id, "X", 250, 240, -1, -1, 0);
                world.component().add<trogue::AttributeComponent>(id, false, true);
            } else {
                world.component().add<trogue::AttributeComponent>(id, true, false);
                world.component().add<trogue::TileComponent>(id, ".", 250, 240, -1, -1, 0);
            }
        }
    }
    for (int i = 0; i < 3; ++i) {
        tyra::EntityId id = world.entity().create();
        world.component().add<trogue::PositionComponent>(id, 1, 1);
        world.component().add<trogue::TileComponent>(id, std::to_string(i), 3+i, 13+i, -1, -1, 2);
        world.component().add<trogue::AttributeComponent>(id, true, false);
    }

    auto pid = world.entity().create();
    world.component().add<trogue::PositionComponent>(pid, map.height()/2, map.width()/2);
    world.component().add<trogue::TileComponent>(pid, "@", 3, 3, -1, -1, 1);
    world.component().add<trogue::SightComponent>(pid, 20);
    world.component().add<trogue::PlayerComponent>(pid);
    world.component().add<trogue::AttributeComponent>(pid, true, false);
    world.component().add<trogue::RenderComponent>(pid, map.height()/2, map.width()/2,"@", 3,-1,1);
    world.tag("PLAYER", pid);

    
    for (int i = 0; i < 1; ++i) {
        auto eid = world.entity().create();
        world.component().add<trogue::PositionComponent>(eid, rand() % map.height(), rand() % map.width());
        world.component().add<trogue::TileComponent>(eid, "E", 5, 6, -1, -1, 1);
        world.component().add<trogue::SightComponent>(eid, 20);
        world.component().add<trogue::AIComponent>(eid, (rand() % 100) + 1100);
        world.component().add<trogue::AttributeComponent>(eid, true, false);
    }
    

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
        std::cout << world.delta() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

}
