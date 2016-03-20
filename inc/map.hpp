#ifndef TROGUE_MAP_H
#define TROGUE_MAP_H


#include <iostream>
#include <vector>
#include <random>



namespace trogue {

    class MapBuilder;

    class Map {
        friend class MapBuilder;

    private:
        int m_height;
        int m_width;

        std::vector<int>    m_tiles;
        std::vector<bool>   m_visible;
        std::vector<bool>   m_walkable;

    protected:
        Map(int height, int width);

    public:
        void tile(int y, int x, int tile);
        int tile(int y, int x) const;
        void visible(int y, int x, bool visible);
        bool visible(int y, int x) const;
        void walkable(int y, int x, bool walkable);
        bool walkable(int y, int x) const;
        int height() const;
        int width() const; 
    };



    class MapBuilder {
    private:
        Map    m_map;

        void floodFill();

    protected:
        bool inRange(int y, int x) const;
        Map& map();
        const Map& map() const;

    public:
        MapBuilder(int height, int width);
        virtual const Map& finalize();
    };



    class AutomataMapBuilder : public MapBuilder {
    private:
        std::vector<bool> m_walls;

        bool isWall(int y, int x) const;
        void isWall(int y, int x, bool val);

    public:
        AutomataMapBuilder(int height, int width, float init_wall_prob);

        void print();
        void build(int r1_limit, int r2_limit);
        const Map& finalize() override;
    };

}
#endif
