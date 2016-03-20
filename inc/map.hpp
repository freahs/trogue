#ifndef TROGUE_MAP_H
#define TROGUE_MAP_H


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include <cmath>
#include <vector>
#include <utility>
#include <random>
#include <functional>

namespace trogue {

#define TILE_FLOOR true
#define TILE_WALL false

    class Map {
    protected:
        int m_height;
        int m_width;
        int m_entry_y;
        int m_entry_x;

        std::vector<int>    m_tiles;
        std::vector<bool>   m_visible_tiles;

        bool inRange(int y, int x) const;
        void floodFill(int y, int x);

    protected:
        void finalize();
        virtual bool isWall(int y, int x) const = 0;

    public:
        Map(int height, int width);

        void set(int row, int col, int i);
        int get(int row, int col) const;

        int height() const;
        int width() const; 
    };

    class AutomataMap : public Map {
    private:
        struct Param {
            int deactivate;
            int activate;
            int num_iterations;
            int neighbour_dist;
            int oob_is_wall;
        };

        std::vector<Param> m_params;

        bool isWall(int y, int x) const override { return true; }

    public:
        using Map::Map;

        void addStep(int deactivate, int activate, int num_iterations,
                int neighbour_dist, bool oob_is_wall) { 
            m_params.push_back({deactivate, activate, num_iterations, neighbour_dist, oob_is_wall});
        }

        int numAdjecantWalls(int cy, int cx, int distance, bool oob_is_wall) const {
            int adj = 0;
            for (int y = -distance; y <= distance; ++y) {
                for (int x = -distance; x <= distance; ++x) {
                    if (y == 0 && x == 0) { continue; }
                    int _y = cy + y;
                    int _x = cx + x;
                    if (_y < 0 || _y >= m_height || _x < 0 || _x >= m_width) {
                        if (oob_is_wall) { ++adj; }
                    } else if (get(_y, _x)) {
                        ++adj;
                    }
                }
            }
            return adj;
        }


        void generate(float wall_prob) {

            std::default_random_engine rng(std::random_device{}());
            std::uniform_real_distribution<float> dist(0.0, 1.0);

            for(int y = 1; y < m_height - 1; ++y) {
                for(int x = 1; x < m_width - 1; ++x) {
                    set(y, x, wall_prob > dist(rng));
                }
            }

            if (m_params.empty()) {
                addStep(4, 4, 7, 1, true);
            }

            for (Param& p : m_params) {
                for (int i = 0; i < p.num_iterations; ++i) { 
                    auto tmp_tiles = m_tiles;
                    for (int y = 1; y < m_height - 1; ++y) {
                        for (int x = 1; x < m_width - 1; ++x) {

                            int adj = numAdjecantWalls(y, x, p.neighbour_dist, p.oob_is_wall);

                            if(get(y,x) && adj < p.deactivate)  { tmp_tiles[y * m_height + x] = false; }
                            else if (get(y,x))                  { tmp_tiles[y * m_height + x] = true; }
                            else if (adj > p.activate)          { tmp_tiles[y * m_height + x] = true; }
                            else                                { tmp_tiles[y * m_height + x] = false;  }
                        }
                    }
                    m_tiles = tmp_tiles;
                }
            }
        }
    };

}
#endif
