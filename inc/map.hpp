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

    class MapBuilder;

    class Map {
    private:
        int m_height;
        int m_width;

        std::vector<int>    m_tiles;
        std::vector<bool>   m_visible;
        std::vector<bool>   m_walkable;

        bool inRange(int y, int x) const;
        void floodFill();

    public:
        Map(int height, int width);

        void tile(int y, int x, int tile);
        int tile(int y, int x) const;
        void visible(int y, int x, bool visible);
        bool visible(int y, int x) const;
        void walkable(int y, int x, bool walkable);
        bool walkable(int y, int x) const;
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

    public:
        AutomataMap(int height, int width)
            : Map(height, width) {
            }

        void addStep(int deactivate, int activate, int num_iterations,
                     int neighbour_dist, bool oob_is_wall) { 
            m_params.push_back({deactivate, activate, num_iterations,
                               neighbour_dist, oob_is_wall});
        }

        int numAdj(int cy, int cx, int distance, bool oob_is_wall,
                   const std::vector<bool>& walls) const {
            int adj = 0;
            for (int y = -distance; y <= distance; ++y) {
                for (int x = -distance; x <= distance; ++x) {
                    if (y == 0 && x == 0) { continue; }
                    int _y = cy + y;
                    int _x = cx + x;
                    if (_y < 0 || _y >= height() || _x < 0 || _x >= width()) {
                        if (oob_is_wall) { ++adj; }
                    } else if (walls[_y*width() + _x]) {
                        ++adj;
                    }
                }
            }
            return adj;
        }


        void generate(float wall_prob) {

            std::default_random_engine rng(std::random_device{}());
            std::uniform_real_distribution<float> dist(0.0, 1.0);

            std::vector<bool> walls(height()*width(), false); 

            for(int y = 1; y < height() - 1; ++y) {
                for(int x = 1; x < width() - 1; ++x) {
                    if (dist(rng) > wall_prob) {
                        walls[y*width() + x] = true;
                    }
                }
            }

            if (m_params.empty()) {
                addStep(4, 4, 7, 1, true);
            }


            for (Param& p : m_params) {
                for (int i = 0; i < p.num_iterations; ++i) { 
                    auto tmp_walls(walls);
                    for (int y = 1; y < height() - 1; ++y) {
                        for (int x = 1; x < width() - 1; ++x) {

                            int adj = numAdj(y, x, p.neighbour_dist, p.oob_is_wall, walls);

                            if(walls[y*width() + x] && adj < p.deactivate) {
                                tmp_walls[y * width() + x] = true;
                            } else if (walls[y*width() + x]) {
                                tmp_walls[y * width() + x] = false;
                            } else if (adj > p.activate) {
                                tmp_walls[y * width() + x] = false;
                            } else {
                                tmp_walls[y * width() + x] = true;
                            }
                        }
                    }
                    walls = tmp_walls;
                }
            }

            for (int y = 0; y < height(); ++y) {
                for (int x = 0; x < width(); ++x) {
                    bool wall = walls[y*width() + x];
                    tile(y, x, wall);
                    walkable(y, x, !wall);
                    visible(y, x, !wall); 
                    std::cout << wall;
                }
                std::cout << std::endl;
            }

        }
    };

}
#endif
