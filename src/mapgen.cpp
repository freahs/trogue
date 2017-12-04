#include <limits>
#include <deque>
#include "mapgen.hpp"

namespace trogue {
    namespace mapgen {

        bool Area::intersects (const Area& a) const {
            if (x1() > a.x2()) return false;
            if (y1() > a.y2()) return false;
            if (x2() < a.x1()) return false;
            if (y2() < a.y1()) return false;
            return true;
        }


        std::pair<int, int> Area::adjacent(const Area& other) const {
            if (other.y2() >= y1() && y2() >= other.y1()) {
                int r_dist = other.x1() - x2() - 1;
                if (r_dist >= 0) return std::make_pair(1, r_dist);
                int l_dist = x1() - other.x2() - 1;
                if (l_dist >= 0) return std::make_pair(3, l_dist);
            } else if (other.x2() >= x1() && x2() >= other.x1()) {
                int top_dist = y1() - other.y2() - 1;
                if (top_dist >= 0) return std::make_pair(0, top_dist);
                int bottom_dist = other.y1() - y2() - 1;
                if (bottom_dist >= 0) return std::make_pair(2, bottom_dist);
            }
            return std::make_pair(-1, std::numeric_limits<int>::max());
        }

        int Area::distance(const Area& a) const {
            int x_dist = abs((x1() + width())/2 - (a.x1() + a.width())/2);
            int y_dist = abs((y1() + height())/2 - (a.y1() + a.height())/2);
            return x_dist + y_dist;
        }

        void Area::move(int rel_y, int rel_x) {
            m_y += rel_y;
            m_x += rel_x;
        }

        bool Area::operator==(const Area& other) const {
            if (m_y != other.m_y || m_x != other.m_x) return false;
            if (m_height != other.m_height || m_width != other.m_width) return false;
            return true;
        }

        std::ostream& operator<<(std::ostream& os, const Area& a) {
            os << "(y = " << a.m_y << ", x = " << a.m_x;
            os << ", h = " << a.m_height << ", w = " << a.m_width << ")";
            return os;
        }

        Graph::Graph(const std::vector<Area>& vec) {
            for (auto& a : vec) {
                m_adj[std::make_shared<Area>(a)] = {};
            }
        }

        std::set<area_ptr>& Graph::adj(const area_ptr ptr) {
            auto it = m_adj.find(ptr);
            if (it == m_adj.end()) {
                throw std::runtime_error("Grap::adj: unknown pointer.");
            }
            return it->second;
        }

        const Graph::set_type& Graph::adj(area_ptr ptr) const {
            auto it = m_adj.find(ptr);
            if (it == m_adj.end()) {
                throw std::runtime_error("Grap::adj: unknown pointer.");
            }
            return it->second;
        }

        void Graph::connect(area_ptr a, area_ptr b) {
            m_adj[a].insert(b);
            m_adj[b].insert(a);
        }

        void Graph::disconnect(area_ptr a, area_ptr b) {
            m_adj[a].erase(b);
            m_adj[b].erase(a);
        }

        void Graph::insert(area_ptr a) {
            if (m_adj.find(a) == m_adj.end()) {
                m_adj[a] = {};
            }
        }

        void Graph::erase(area_ptr a) {
            auto it = m_adj.find(a);
            if (it == m_adj.end()) { return; }
            for (auto& b : it->second) {
                m_adj[b].erase(a);
            }
            m_adj.erase(a);
        }

        std::set<area_ptr> Graph::areas() const {
            std::set<area_ptr> s;
            for (auto& p : m_adj) {
                s.insert(p.first);
            }
            return s;
        }

        // Use binary space partitioning to generate areas according to parameres.
        Graph bsp(int height, int width, int min_size) {
            std::deque<Area> q;
            std::vector<Area> v;
            using Type = Area::Type;
            q.push_back(Area(0, 0, height, width, Area::Type::ROOM));
            while (!q.empty()) {
                auto a = q.front();
                q.pop_front();
                if (a.height() > a.width() && a.height() >= min_size*2) {
                    auto offset = trogue::utils::random(min_size, a.height() - min_size);
                    q.push_back(
                        Area(a.y1(), a.x1(), offset, a.width(), Type::ROOM));
                    q.push_back(
                        Area(a.y1() + offset, a.x1(), a.height() - offset, a.width(), Type::ROOM));
                } else if (a.width() >= a.height() && a.width() >= min_size*2) {
                    auto offset = trogue::utils::random(min_size, a.width() - min_size);
                    q.push_back(
                        Area(a.y1(), a.x1(), a.height(), offset, Type::ROOM));
                    q.push_back(
                        Area(a.y1(), a.x1() + offset, a.height(), a.width() - offset, Type::ROOM));
                } else {
                    v.push_back(a);
                }
            }
            return Graph(v);
        }

        // Creates a room of corridor type connecting two areas. The corridors relative width
        // will be bounded by the shared coordinates of the areas. If there are no shared
        // the corridor will connect to the closest corners of the two areas and it's relative
        // width will be the distance between the areas + 2. The relative height of the corridor
        // will always be the distance between the areas along the opposite axis of the shared
        // coordinates
        area_ptr create_corridor(area_ptr a, area_ptr b) {
            using Type = Area::Type;

            // To minimize the number of possible configurations, make sure
            // A is above B
            if (b->y2() < a->y1()) { std::swap(a, b); }

            // Vertical alignment
            if (a->y2() < b->y1() - 1) {
                int y = a->y2() + 1;
                int h = b->y1() - a->y2() - 1;

                // Since height and y-coord is done, make sure A is to the left of B
                // to reduce possiblie configurations.
                if (a->x2() > b->x2()) { std::swap(a, b); }
                if (a->x2() < b->x1()) {
                    int w = b->x1() - a->x2() + 1;
                    return std::make_shared<Area>(y, a->x2(), h, w, Type::CORRIDOR);
                } else if (a->x1() <= b->x1() && a->x2() <= b->x2()) {
                    int w = a->x2() - b->x1() + 1;
                    return std::make_shared<Area>(y, b->x1(), h, w, Type::CORRIDOR);
                } else {
                    return std::make_shared<Area>(y, a->x1(), h, a->width(), Type::CORRIDOR);
                }
            }

            // Repeat as above but for horizontal alignment
            if (b->x2() < a->x1()) { std::swap(a, b); }
            if (a->x2() < b->x1() - 1) {
                int x = a->x2() + 1;
                int w = b->x1() - a->x2() - 1;

                if (a->y2() > b->y2()) { std::swap(a, b); }
                if (a->y2() < b->y1()) {
                    int h = b->y1() - a->y2() + 1;
                    return std::make_shared<Area>(a->y2(), x, h, w, Type::CORRIDOR);
                } else if (a->y1() <= b->y1() && a->y2() <= b->y2()) {
                    int h = a->y2() - b->y1() + 1;
                    return std::make_shared<Area>(b->y1(), x, h, w, Type::CORRIDOR);
                } else {
                    return std::make_shared<Area>(a->y1(), x, a->height(), w, Type::CORRIDOR);
                }
            }

            return nullptr;
        }

        // Utilizes the create_corridor function to create a door of specific size 
        area_ptr create_door(area_ptr a, area_ptr b, int wall_size, int door_width) {
            auto adj = a->adjacent(*b);
            if (adj.first < 0 || adj.second > wall_size) { return nullptr; }

            auto door = create_corridor(a, b);
            if (door == nullptr) { return nullptr; }

            if (adj.first % 2 == 0 ) /* vertical door */ {
                auto mod = door->width() - door_width;
                door->move(0, mod < 0 ? 0 : utils::random(0, mod));
                if (door->width() > door_width) {
                    door->width(door_width);
                }
            } else /* horizontal door */ {
                auto mod = door->height() - door_width;
                door->move(mod < 0 ? 0 : utils::random(0, mod), 0);
                if (door->height() > door_width) {
                    door->height(door_width);
                }
            }
            return door;
        }

        // Make sure all areas are separated by at least as much wall as specified by wall_size
        // and adds doors with width door_width between all appropriate rooms.
        void create_rooms(Graph& g, int wall_size, int door_width) {
            using Type = Area::Type;
            auto areas = g.areas();
            for (auto& a : areas) {
                for (auto& b : areas) {
                    auto adj = a->adjacent(*b);
                    if (adj.second == 0 && adj.first == 1) a->move_right_wall(wall_size);
                    if (adj.second == 0 && adj.first == 2) a->move_bottom_wall(wall_size);
                }
            }
            for (auto& a : areas) {
                for (auto& b : areas) {
                    auto door = create_door(a, b, wall_size, door_width);
                    if (a < b && door != nullptr) {
                        g.connect(a, door);
                        g.connect(b, door);
                    }
                }
            }
        }

        // Creates a minimum spanning tree from a graph.
        Graph mst(const Graph& g) {
            Graph mst;
            auto areas = g.areas();
            mst.insert(*areas.begin());
            areas.erase(areas.begin());
            while (!areas.empty()) {
                area_ptr best_u = nullptr;
                area_ptr best_v = nullptr;
                int best_d = std::numeric_limits<int>::max();
                for (auto& u : mst.areas()) {
                    for (auto& v : g.adj(u)) {
                        if (u->distance(*v) < best_d && u != v && areas.find(v) != areas.end()) {
                            best_d = u->distance(*v);
                            best_u = u;
                            best_v = v;
                        }
                    }
                }
                if (best_u == nullptr) { break; }
                mst.connect(best_u, best_v);
                areas.erase(best_v);
            }
            for (auto& p : mst) {
                if (p.first->type() == Area::Type::CORRIDOR && p.second.size() < 2) {
                    mst.erase(p.first);
                }
            }
            return mst;
        }
    }
}
