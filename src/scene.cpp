#include "scene.hpp"

#include "tyra/tyra.hpp"
#include "ansi_format.hpp"

#include "components.hpp"

#include <algorithm>
#include <iterator>
#include <vector>

namespace trogue {

    Scene::EntityStack::EntityStack()
        : m_current(m_entities.end()) {
        }

    void Scene::EntityStack::update() {
        if (m_entities.size() != 0) {
            ++m_current;
            if (m_current == m_entities.end()) {
                m_current = m_entities.begin();
            }
        }
    }

    bool Scene::EntityStack::add(tyra::EntityId id) {
        auto res = m_entities.insert(id);
        m_current = res.first;
        return res.second;
    }

    bool Scene::EntityStack::remove(tyra::EntityId id) {
        if (m_current != m_entities.end()) {
            tyra::EntityId current = *m_current;
            auto next = std::next(m_current, 1);
            auto res = m_entities.erase(id);
            if (current == id) {
                if (next == m_entities.end()) {
                    m_current = m_entities.begin();
                } else {
                    m_current = next;
                }
            }
            return res;
        }
        return false;
    }

    tyra::EntityId Scene::EntityStack::get() const {
        if (m_current == m_entities.end()) {
            return -1;
        } else {
            return *m_current;
        }
    }

    Scene::Scene(Map<bool> map, int num_layers)
        : m_num_layers(num_layers),
        m_center_y(-1), m_center_x(-1),
        //m_delta(0), m_last_update(Time::now()),
        m_elapsed_time(0),
        m_stacks(map.height() * map.width() * m_num_layers),
        m_terrain_map(map),
        m_shadowcast(m_terrain_map) {
        }

    Scene::EntityStack& Scene::getStack(int y, int x, int layer) {
        return m_stacks[y*width()*layers() + x*layers() + layer];
    }

    const Scene::EntityStack& Scene::getStack(int y, int x, int layer) const {
        return m_stacks[y*width()*layers() + x*layers() + layer];
    }

    bool Scene::visible(int y, int x) const {
        return m_shadowcast.visible(y, x);
    }

    bool Scene::visited(int y, int x) const {
        return m_shadowcast.visited(y, x);
    }

    bool Scene::blocked(int y, int x) const {
        return m_terrain_map[y][x];
    }

    void Scene::update(int center_y, int center_x, int range, int delta) {
        if (m_center_y != center_y || m_center_x != center_x) {
            m_center_y = center_y;
            m_center_x = center_x;
            m_shadowcast.update(center_y, center_x, range);
        }

        m_elapsed_time += delta;

        if (m_elapsed_time > s_tile_rotation_time) {
            m_elapsed_time = 0;
            for (auto &s : m_stacks) {
                s.update();
            }
        }
    }

    void Scene::add(int y, int x, int layer, tyra::EntityId id) {
        EntityStack& stack = getStack(y, x, layer);
        stack.add(id);
    }

    void Scene::remove(int y, int x, int layer, tyra::EntityId id) {
        EntityStack& stack = getStack(y, x, layer);
        stack.remove(id);
    }

    tyra::EntityId Scene::get(int y, int x, int layer) const {
        return getStack(y, x, layer).get();
    }

    int Scene::width() const {
        return m_terrain_map.width();
    }

    int Scene::height() const {
        return m_terrain_map.height();
    }

    int Scene::layers() const {
        return m_num_layers;
    }

    int Scene::centerY() const {
        return m_center_y;
    }

    int Scene::centerX() const {
        return m_center_x;
    }

    bool Scene::inRange(int y, int x) const {
        return m_terrain_map.inRange(y, x);
    }

    void Scene::print() const {
        for (int y = 0; y < height(); ++y) {
            for (int x = 0; x < width(); ++x) {
                if (y == m_center_y && x == m_center_x) {
                    std::cout << format::fg(4) << "@";
                } else if(visible(y,x)) {
                    if (!m_terrain_map[y][x]) {
                        std::cout << format::fg(255) << ".";
                    } else {
                        std::cout << format::fg(255) << "X";
                    }
                } else if (visited(y, x)) {
                    if (!m_terrain_map[y][x]) {
                        std::cout << format::fg(243) << ".";
                    } else {
                        std::cout << format::fg(243) << "X";
                    }
                } else {
                    std::cout << format::fg(0) << " ";
                }
            }
            std::cout << std::endl;
        }
    }

}
