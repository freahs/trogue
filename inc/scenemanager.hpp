#ifndef TROGUE_H
#define TROGUE_H

#include "tyra/tyra.hpp"

#include <set>
#include <vector>

namespace trogue {

    struct Tile {
        const tyra::EntityId    id;
        const int               layer;
        Tile(tyra::EntityId id, int layer) : id(id), layer(layer) { }
    };

    class TileStack {
    private:
        std::vector<Tile> m_tiles;

    public:
        void add(tyra::EntityId id, int layer);
        bool remove(tyra::EntityId id);
        std::vector<Tile>::const_iterator begin() const;
        std::vector<Tile>::const_iterator end() const;
    };

    class SceneManager : public tyra::Manager {
    private:
        std::vector<std::vector<TileStack>> m_tiles;

    public:
        void add(int x, int y, tyra::EntityId id, int layer);
        bool remove(int x, int y, tyra::EntityId id);
        std::vector<std::vector<TileStack>>::const_iterator begin() const;
        std::vector<std::vector<TileStack>>::const_iterator end() const;
    };
}

#endif
