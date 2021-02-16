#ifndef __AOC_DAY_24__
#define __AOC_DAY_24__

#include <string>

#include "aoc_day.h"
#include "screen.h"

#define DIRECTION_EAST 0
#define DIRECTION_SOUTHEAST 1
#define DIRECITON_SOUTHWEST 2
#define DIRECITON_WEST 3
#define DIRECTION_NORTHWEST 4
#define DIRECTION_NORTHEAST 5

#define CHAR_EAST 'e'
#define CHAR_WEST 'w'
#define CHAR_NORTH 'n'
#define CHAR_SOUTH 's'

using namespace std;

struct Day24Tile
{
    string input;
    int x;
    int y;
};

class AocDay24 : public AocDay
{
    private:
        vector<string> read_input(string filename);
        vector<Day24Tile> make_tiles(vector<string> inputs);
        void determine_grid_sizes(vector<Day24Tile> & tiles, int buffer, int & min_x, int & max_x, int & min_y, int & max_y);
        void init_white_tiles(Screen & hex_grid, int min_x, int max_x, int min_y, int max_y);
        void flip_tile(Screen & hex_grid, Day24Tile tile);
        int get_num_black_neighbors(Screen & hex_grid, int x, int y);
        vector<pair<int, int>> get_points_to_set_to_black(Screen & hex_grid, int min_x, int max_x, int min_y, int max_y);
        void determine_next_grid_size(Screen & hex_grid, int buffer, int & min_x, int & max_x, int & min_y, int & max_y);
    public:
        AocDay24();
        ~AocDay24();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
