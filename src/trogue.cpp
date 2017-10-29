#define _XOPEN_SOURCE_EXTENDED
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include "map.hpp"
#include "scene.hpp"
#include "input.hpp"
#include "shadowcast.hpp"
#include "tilesystem.hpp"
#include "movementsystem.hpp"
#include "playersystem.hpp"
#include "aisystem.hpp"
#include "components.hpp"
#include "tile.hpp"
#include "visibilitysystem.hpp"

#include "tyra/tyra.hpp"

typedef std::chrono::high_resolution_clock      Time;
typedef std::chrono::milliseconds               Ms;
typedef std::chrono::system_clock::time_point   TimePoint;

int map_height = 30;
int map_width = 30;

void create_player(tyra::World& world) {
    auto n_tile = trogue::Tile::create("@", 1, -1);
    auto h_tile = trogue::Tile::create("@", 2, -1);
    auto pid = world.entity().create();
    world.component().add<trogue::PositionComponent>(pid, map_height/2, map_width/2);
    world.component().add<trogue::TileComponent>(pid, n_tile, h_tile, 1);
    world.component().add<trogue::SightComponent>(pid, 20);
    world.component().add<trogue::PlayerComponent>(pid);
    world.component().add<trogue::VisibleComponent>(pid);
    world.tag("PLAYER", pid);
}

trogue::Map<bool> create_map() {
    trogue::Map<bool> map(map_width, map_height);
    map[map.height()/2 + 1][map.width()/2 + 1] = true;
    map[map.height()/2 + 2][map.width()/2 + 1] = true;
    map[map.height()/2 + 3][map.width()/2 + 1] = true;
    map[map.height()/2 + 4][map.width()/2 + 1] = true;
    return map;
}

void create_tiles(const trogue::Map<bool> map, tyra::World& world) {
    auto wall_n_tile = trogue::Tile::create("X", 250, -1);
    auto wall_b_tile = trogue::Tile::create("X", 240, -1);
    auto floor_n_tile = trogue::Tile::create(".", 250, -1);
    auto floor_b_tile = trogue::Tile::create(".", 240, -1);
    for (int y = 0; y < map.height(); ++y) {
        for (int x = 0; x < map.width(); ++x) {
            tyra::EntityId id = world.entity().create();
            world.component().add<trogue::PositionComponent>(id, y, x);
            if (map[y][x]){
                world.component().add<trogue::TileComponent>(id, wall_n_tile, wall_b_tile, 0);
            } else {
                world.component().add<trogue::TileComponent>(id, floor_n_tile, floor_b_tile, 0);
            }
        }
    }
}

void create_some_stuff(tyra::World& world) {
    for (int i = 0; i < 3; ++i) {
        tyra::EntityId id = world.entity().create();
        auto normal = trogue::Tile::create(std::to_string(i), 3+i, -1);
        auto blocked = trogue::Tile::create(std::to_string(i), 13+i, -1);
        world.component().add<trogue::PositionComponent>(id, 1, 1);
        world.component().add<trogue::TileComponent>(id, normal, blocked, 2);
    }
}

void create_some_enemies(tyra::World& world) {
    auto normal = trogue::Tile::create("E", 30, -1);
    auto blocked = trogue::Tile::create("E", 40, -1);
    for (int i = 0; i < 3; ++i) {
        auto eid = world.entity().create();
        world.component().add<trogue::PositionComponent>(eid, rand() % map_height, rand() % map_width);
        world.component().add<trogue::TileComponent>(eid, normal, blocked, 1);
        world.component().add<trogue::SightComponent>(eid, 20);
        world.component().add<trogue::AIComponent>(eid, (rand() % 100) + 100);
    }
}

int main() {

    auto map = create_map();
    trogue::Scene scene(map, 5);


    tyra::World world;
    world.system().add<trogue::AISystem>();
    world.system().add<trogue::MovementSystem>(scene);
    world.system().add<trogue::VisibilitySystem>(scene);
    world.system().add<trogue::PlayerSystem>(scene);
    world.system().add<trogue::TileSystem>(scene, *trogue::Display::instance(), map.height(), map.width());
    world.entity().create();

    create_player(world);
    create_tiles(map, world);
    create_some_stuff(world);
    create_some_enemies(world);

    trogue::Input input;
    trogue::Key res = trogue::Key::NONE;

    world.start();
    auto pid = world.tag("PLAYER");
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

}
