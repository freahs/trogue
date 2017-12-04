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
#include "terrainsystem.hpp"
#include "animationsystem.hpp"
#include "mapgen.hpp"
#include "utils.hpp"
#include "ansi_format.hpp"

#include "tyra/tyra.hpp"

#include "ecs.hpp"

typedef std::chrono::high_resolution_clock      Time;
typedef std::chrono::milliseconds               Ms;
typedef std::chrono::system_clock::time_point   TimePoint;

static const int map_height = 50;
static const int map_width = 50;

void create_animation(tyra::World& world) {
    auto id = world.entity().create();
    world.component().add_as<trogue::AnimationComponent, trogue::FireAnimationComponent>(id);
    auto ac = &world.component().get<trogue::AnimationComponent>(id);
    world.component().add_as<trogue::TileComponent, trogue::AnimationTileComponent>(id, &ac->normal_ptr, &ac->blocked_ptr, 5);
    world.component().add<trogue::PositionComponent>(id, 5, 5);
}

void create_player(tyra::World& world) {
    auto n_tile = trogue::Tile::create("@", 0xFFFF00, -1);
    auto h_tile = trogue::Tile::create("@", 0xAAAA00, -1);
    auto pid = world.entity().create();
    world.component().add<trogue::PositionComponent>(pid, map_height/2, map_width/2);
    world.component().add_as<trogue::TileComponent, trogue::SharedTileComponent>(pid, n_tile, h_tile, 1);
    world.component().add<trogue::SightComponent>(pid, 20);
    world.component().add<trogue::PlayerComponent>(pid);
    world.component().add<trogue::AttributeComponent>(pid, trogue::Attribute::SOLID);
    world.tag("PLAYER", pid);
}

void create_tiles(tyra::World& world) {
    auto wall_n_tile = trogue::Tile::create("X", 0x606060, -1);
    auto wall_b_tile = trogue::Tile::create("X", 0x505050, -1);
    auto floor_n_tile = trogue::Tile::create(".", 0x707070, -1);
    auto floor_b_tile = trogue::Tile::create(".", 0x505050, -1);
    for (int y = 0; y < map_height; ++y) {
        for (int x = 0; x < map_width; ++x) {
            tyra::EntityId id = world.entity().create();
            world.component().add<trogue::PositionComponent>(id, y, x);
            if (y == map_height/2 + 2 && x > 3 && x < map_width - 3){
                world.component().add_as<trogue::TileComponent, trogue::SharedTileComponent>(id, wall_n_tile, wall_b_tile, 0);
                world.component().add<trogue::AttributeComponent>(id, trogue::Attribute::SOLID, trogue::Attribute::OPAQUE);
            } else {
                world.component().add_as<trogue::TileComponent, trogue::SharedTileComponent>(id, floor_n_tile, floor_b_tile, 0);
            }
        }
    }
}

void create_some_stuff(tyra::World& world) {
    for (int i = 0; i < 3; ++i) {
        tyra::EntityId id = world.entity().create();
        auto normal = trogue::Tile::create(std::to_string(i), 0xFF << 4*i | 0x888888, -1);
        auto blocked = trogue::Tile::create(std::to_string(i), 0xFF << 5*i | 0x666666, -1);
        world.component().add<trogue::PositionComponent>(id, 1, 1);
        world.component().add_as<trogue::TileComponent, trogue::SharedTileComponent>(id, normal, blocked, 2);
    }
}

void create_some_enemies(tyra::World& world) {
    auto normal = trogue::Tile::create("E", 0x30FFFF, -1);
    auto blocked = trogue::Tile::create("E", 0x10FFFF, -1);
    for (int i = 0; i < 3; ++i) {
        auto eid = world.entity().create();
        world.component().add<trogue::PositionComponent>(eid, rand() % map_height, rand() % map_width);
        world.component().add_as<trogue::TileComponent, trogue::SharedTileComponent>(eid, normal, blocked, 1);
        world.component().add<trogue::SightComponent>(eid, 20);
        world.component().add<trogue::AIComponent>(eid, (rand() % 100) + 1000);
        world.component().add<trogue::AttributeComponent>(eid, trogue::Attribute::SOLID, trogue::Attribute::OPAQUE);
    }
}

void print_area(trogue::mapgen::Area a, int color, int max_height, int max_width) {
    for (int i = 0; i < a.height(); ++i) {
        for (int j = 0; j < a.width(); ++j) {
            std::cout << format::pos(a.y1() + i + 1, a.x1() + j + 1) << format::bg(color) << ".";
        }
    }
    std::cout << format::pos(max_height, max_width) << format::clear << std::endl;
}

void rprint_area(trogue::mapgen::Area a, int color) {
    std::cout << format::rpos(a.y1(), 0);
    for (int i = 0; i < a.height(); ++i) {
        std::cout << format::rpos(0, a.x1()) << format::bg(color) << std::string(a.width(), '.') << std::endl;
    }
    std::cout << format::rpos(-a.y1() - a.height(), -a.x1() - a.width()) << format::clear;
}

int main() {
    trogue::TWorld world(map_height, map_width);
    world.system().add<trogue::PlayerSystem>();
    world.system().add<trogue::AISystem>();
    world.system().add<trogue::AnimationSystem>();
    world.system().add<trogue::MovementSystem>();
    world.system().add<trogue::VisibilitySystem>();
    world.system().add<trogue::TileSystem>(*trogue::Display::instance(), map_height, map_width);
    world.entity().create();

    create_player(world);
    create_tiles(world);
    create_some_stuff(world);
    create_some_enemies(world);
    create_animation(world);

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
