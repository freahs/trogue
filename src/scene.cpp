#include "scene.hpp"

#include "tyra/tyra.hpp"
#include "ansi_format.hpp"

#include "components.hpp"

#include <algorithm>
#include <iterator>
#include <vector>

namespace trogue {


    Scene::Scene(int height, int width)
    : m_height(height), m_width(width),
    m_elapsed_time(s_tile_rotation_time),
    m_stacks(height, width) { 
    }

    void Scene::update(int delta) {

        m_elapsed_time += delta;

        if (m_elapsed_time >= s_tile_rotation_time) {
            m_elapsed_time = 0;
            for (auto &s : m_stacks) {
                s.rotate();
            }
        }
        for (auto &s : m_stacks) {
            s.update();
        }
    }

    void Scene::add(int y, int x, int layer, tyra::EntityId id) {
        m_stacks[y][x].add(id, layer);
    }

    void Scene::remove(int y, int x, tyra::EntityId id) {
        m_stacks[y][x].remove(id);
    }

    tyra::EntityId Scene::get(int y, int x) const {
        return m_stacks[y][x].get();
    }

    const EntityStack& Scene::all(int y, int x) const {
        return m_stacks[y][x];
    }

    int Scene::width() const {
        return m_width;
    }

    int Scene::height() const {
        return m_height;
    }

    bool Scene::inRange(int y, int x) const {
        if (y < 0) return false;
        if (y >= m_height) return false;
        if (x < 0) return false;
        if (x >= m_width) return false;
        return true;
    }

}
