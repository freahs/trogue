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

    Scene::Scene(int height, int width)
        : m_height(height),
        m_width(width),
        m_stacks(m_height * m_width) {
        }

    Scene::EntityStack& Scene::getStack(int row, int col, int layer) {
        return m_stacks[row*m_width + col][layer];
    }

    const Scene::EntityStack& Scene::getStack(int row, int col, int layer) const {
        return m_stacks[row*m_width + col][layer];
    }

    void Scene::update() {
        for (LayerStack& layer : m_stacks) {
            for (EntityStack& stack : layer) {
                stack.update();
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

}
