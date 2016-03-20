#include "../inc/scene.hpp"

#include "../inc/tyra/tyra.hpp"

#include "../inc/components.hpp"

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
        return res.second;
    }

    bool Scene::EntityStack::remove(tyra::EntityId id) {
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

    tyra::EntityId Scene::EntityStack::get() const {
        if (m_current == m_entities.end()) {
            return -1;
        } else {
            return *m_current;
        }
    }

    Scene::Scene(Map map)
        : m_center_y(-1), m_center_x(-1),
        m_delta(0),
        m_last_update(Time::now()),
        m_stacks(map.height() * map.width()),
        m_visited(map.height() * map.width(), false),
        m_shadowcast(max_visibility_range),
        m_map(map) {
        }

    Scene::EntityStack& Scene::getStack(int row, int col, int layer) {
        return m_stacks[row*width() + col][layer];
    }

    const Scene::EntityStack& Scene::getStack(int row, int col, int layer) const {
        return m_stacks[row*width() + col][layer];
    }

    bool Scene::visible(int row, int col) const {
        if (row > 0 && row < height() && col > 0 && col < height()) {
            int rel_y = row - m_center_y;
            int rel_x = col - m_center_x;
            if (m_shadowcast.get(rel_y, rel_x)) {
                m_visited[row*width() + col] = true;
                return true;
            }
        }
        return false;
    }

    void Scene::update(int y, int x) {
        m_delta += std::chrono::duration_cast<Ms>(Time::now() - m_last_update).count();

        if (m_center_y != y || m_center_x != x) {
            m_center_y = y;
            m_center_x = x;
            m_shadowcast.reset(m_shadowcast.size());
            for (int rel_y = -m_shadowcast.size(); rel_y <= m_shadowcast.size(); ++y) {
                for (int rel_x = -m_shadowcast.size(); rel_x <= m_shadowcast.size(); ++x) {
                    int row = m_center_y - rel_y;
                    int col = m_center_x - rel_x;
                    bool in_range = row > 0 && row < height() && col > 0 && col < height();
                    if (!in_range || !m_map.visible(row, col)) {
                        m_shadowcast.set(rel_y, rel_x);
                    }
                }
            }

        }

        if (m_delta > tile_rotation_time) {
            m_delta = m_delta - tile_rotation_time;

            for (LayerStack& layer : m_stacks) {
                for (EntityStack& stack : layer) {
                    stack.update();
                }
            }
        }
    }

    void Scene::add(int row, int col, size_t layer, tyra::EntityId id) {
        EntityStack& stack = getStack(row, col, layer);
        stack.add(id);
    }

    void Scene::remove(int row, int col, size_t layer, tyra::EntityId id) {
        EntityStack& stack = getStack(row, col, layer);
        stack.remove(id);
    }

    tyra::EntityId Scene::get(int row, int col, size_t layer) const {
        return getStack(row, col, layer).get();
    }

    int Scene::width() const {
        return m_map.width();
    }

    int Scene::height() const {
        return m_map.height();
    }

}
