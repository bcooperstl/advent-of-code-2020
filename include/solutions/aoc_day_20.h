#ifndef __AOC_DAY_20__
#define __AOC_DAY_20__

#include "aoc_day.h"

#define TILE_CHAR_OFF '.'
#define TILE_CHAR_ON '#'

#define TILE_SIDE_LEN 10

#define NUM_BORDERS 8

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

enum Manipulations
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
        char m_map[TILE_SIDE_LEN][TILE_SIDE_LEN];
        int m_borders[NUM_BORDERS];
    public:
        Tile(int id);
        ~Tile();
        void display();
        
};

class AocDay20 : public AocDay
{
    private:
        void parse_input(string filename, vector<Tile *> & tiles);
    public:
        AocDay20();
        ~AocDay20();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};

#endif
