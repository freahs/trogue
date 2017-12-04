#ifndef TROGUE_MAPGEN_H
#define TROGUE_MAPGEN_H

#include <iostream>
#include <memory>
#include <cmath>
#include <utility>
#include <map>
#include <set>

#include "utils.hpp"

namespace trogue {
    namespace mapgen {

        class Area {
        public:
            enum class Type {NONE, ROOM, CORRIDOR};

        private:
            int m_y, m_x, m_height, m_width;
            Type m_type = Type::NONE;

        public:
            Area()
            : m_y(0), m_x(0), m_height(0), m_width(0), m_type(Type::NONE) {}
            Area(int y, int x, int height, int width, Type type)
            : m_y(y), m_x(x), m_height(height), m_width(width), m_type(type) {}

            bool intersects(const Area& a) const;
            std::pair<int, int> adjacent( const Area& a) const;
            int distance(const Area& a) const;
            int y1() const { return m_y; }
            int x1() const { return m_x; }
            int y2() const { return m_y + m_height - 1; }
            int x2() const { return m_x + m_width - 1; }
            int width() const { return m_width; }
            int height() const { return m_height; }
            void width(int width) { m_width = width; }
            void height(int height) { m_height = height; }
            Type type() const { return m_type;}

            void move(int y, int x);
            void move_top_wall(int dist) { m_y += dist; m_height -= dist; }
            void move_right_wall(int dist) { m_width -= dist; }
            void move_bottom_wall(int dist) { m_height -= dist; }
            void move_left_wall(int dist) { m_x += dist; m_width -= dist; }

            bool operator==(const Area& other) const;

            friend std::ostream& operator<<(std::ostream& os, const Area& a);
        };

        using area_ptr = std::shared_ptr<Area>;

        class Graph {
            using set_type = std::set<area_ptr>;
            using map_type = std::map<area_ptr, set_type>;
            map_type m_adj;

        public:
            Graph() = default;
            Graph(const std::vector<Area>& vec);
            set_type& adj(area_ptr ptr);
            const set_type& adj(area_ptr ptr) const;
            void connect(area_ptr a, area_ptr b);
            void disconnect(area_ptr a, area_ptr b);
            void insert(area_ptr a);
            void erase(area_ptr a);
            map_type::iterator begin() { return m_adj.begin(); }
            map_type::iterator end() { return m_adj.end(); }
            std::set<area_ptr> areas() const;
        };
        area_ptr create_corridor(area_ptr a1, area_ptr a2);

        Graph bsp(int h, int w, int min_size);
        area_ptr create_door(area_ptr a, area_ptr b);
        void create_rooms(Graph& g, int wall_size, int door_width);
        Graph mst(const Graph& g);
        /*
        std::vector<std::shared_ptr<Area>> bsp(int height, int width, int min_partition_size);
        void add_space(std::vector<Area>& v, int dist);
        std::vector<area_ptr> add_doors(std::vector<area_ptr>& v);
        std::vector<area_ptr> bsp2(std::vector<area_ptr> areas);
        */


        /*
        namespace impl {
            class bsp : public Area {
                std::unique_ptr<bsp> l;
                std::unique_ptr<bsp> r;

                bsp (int y, int x, int h, int w)
                : Area(y, x, h, w, Area::Type::ROOM), l(nullptr), r(nullptr) { }

                void split(int min_size);
                void areas(std::vector<area_ptr>& v) const;

                friend std::vector<area_ptr> trogue::mapgen::bsp(int h, int w, int min_size);
                friend Graph trogue::mapgen::bsp3(int h, int w, int min_size);
            };
        }
        */

    }
}


#endif
