#include "../inc/scenemanager.hpp"

#include <algorithm>

namespace trogue {

    void TileStack::add(tyra::EntityId id, int layer) {
        m_tiles.push_back(Tile(id, layer));
        std::sort(begin(), end(), [](const Tile& a, const Tile& b)->bool {
                return a.layer < b.layer;
                });
    }

    bool TileStack::remove(const tyra::EntityId id) {
        auto it = find_if(begin(), end(), [&](const Tile& tile) {
                return tile.id == id;
                });

        if (it != end()) {
            m_tiles.erase(it);
            return true;
        }
        return false;
    }

    std::vector<Tile>::const_iterator TileStack::begin() const {
        return m_tiles.begin();
    }

    std::vector<Tile>::const_iterator TileStack::end() const {
        return m_tiles.end();
    }

    void SceneManager::add(int x, int y, tyra::EntityId id, int layer) {
        m_tiles[y][x].add(id, layer);
    }

    bool SceneManager::remove(int x, int y, tyra::EntityId id) {
        return m_tiles[y][x].remove(id);
    }

    std::vector<std::vector<TileStack>>::const_iterator SceneManager::begin() const {
        return m_tiles.begin();
    }

    std::vector<std::vector<TileStack>>::const_iterator SceneManager::end() const {
        return m_tiles.end();
    }
}
