#include "scene.hpp"

#include "tyra/tyra.hpp"
#include "ansi_format.hpp"

#include "components.hpp"

#include <algorithm>
#include <iterator>
#include <vector>

namespace trogue {


    Scene::Scene(int height, int width)
    : m_center_y(-1), m_center_x(-1),
    m_elapsed_time(0),
    m_stacks(height, width), 
    m_shadowcast(height, width) {
    }

    void Scene::opaque(int row, int col, bool opaque) {
        m_shadowcast.opaque(row, col, opaque);
    }

    bool Scene::visible(int y, int x) const {
        if (y == m_center_y && x == m_center_x) {
            return true;
        }
        return m_shadowcast.visible(y, x);
    }

    bool Scene::visited(int y, int x) const {
        return m_shadowcast.visited(y, x);
    }

    void Scene::update(int center_y, int center_x, int range, int delta) {
        m_center_y = center_y;
        m_center_x = center_x;
        m_shadowcast.update(center_y, center_x, range);

        m_elapsed_time += delta;

        if (m_elapsed_time > s_tile_rotation_time) {
            m_elapsed_time = 0;
            for (auto &s : m_stacks) {
                s.update();
            }
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
        return m_shadowcast.width();
    }

    int Scene::height() const {
        return m_shadowcast.height();
    }

    int Scene::centerY() const {
        return m_center_y;
    }

    int Scene::centerX() const {
        return m_center_x;
    }

    bool Scene::inRange(int y, int x) const {
        return m_shadowcast.inRange(y, x);
    }

}
