#ifndef __AOC_DAY_20__
#define __AOC_DAY_20__

#include "aoc_day.h"

#include <vector>
#include <map>

#define TILE_CHAR_OFF '.'
#define TILE_CHAR_ON '#'

#define TILE_SIDE_LEN 10

#define TILE_NUM_BORDERS 8

enum Border
{
    NorthFront=0,
    EastFront=1,
    SouthFront=2,
    WestFront=3,
    NorthBack=4,
    EastBack=5,
    SouthBack=6,
    WestBack=7
};

enum Manipulation
{
    RightRotate0Degrees=0,
    RightRotate90Degrees=1,
    RightRotate180Degrees=2,
    RightRotate270Degrees=3,
    HorizontalFlip=4,
    VerticalFlip=5,
    ForwardSlashFlip=6,
    BackwardSlashFlip=7
};

class Tile
{
    private:
        int m_id;
        char m_map[TILE_SIDE_LEN][TILE_SIDE_LEN]; // first index is row top-to-bottom, second index is column left-to-right
        int m_borders[TILE_NUM_BORDERS];
        void build_border_values();
    public:
        Tile(int id);
        ~Tile();
        void set_map(vector<string> map_lines);
        void display();
        int get_id();
        vector<string> get_tile_map(); // used for testing.
        int get_border(Border border);
        int get_min_border(Border border1, Border border2);
        map<Border, int> get_borders();
        void perform_manipulation(Manipulation manipulation);
        
};

class AocDay20 : public AocDay
{
    private:
        void parse_input(string filename, vector<Tile *> & tiles);
        void build_border_lookup_map(vector<Tile *> & tiles, map<int, vector<Tile *>> & lookup_map);
        void classify_tiles(vector<Tile *> & tiles, vector<Tile *> & corners, vector<Tile *> & edges, vector<Tile *> & middles, map<int, vector<Tile *>> & lookup_map);
    public:
        AocDay20();
        ~AocDay20();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
