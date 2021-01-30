#ifndef __AOC_DAY_20__
#define __AOC_DAY_20__

#include "aoc_day.h"

#include <vector>
#include <map>

#define TILE_CHAR_OFF '.'
#define TILE_CHAR_ON '#'
#define IMAGE_CHAR_MONSTER 'O'

#define TILE_SIDE_LEN 10

#define TILE_NUM_BORDERS 8

#define PUZZLE_MAX_SIDE_LEN 12

#define PUZZLE_NUM_IMAGES 8

#define NUM_MONSTER_PIXELS 15

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
        vector<string> get_tile_map(); // used for testing and displaying puzzles.
        int get_border(Border border);
        int get_paired_border(Border border);
        int get_min_border(Border border1, Border border2);
        map<Border, int> get_borders();
        void perform_manipulation(Manipulation manipulation);
        Border get_border_for_value(int value);
};

class Puzzle
{
    private:
        int m_side_len;
        int m_picture_side_len;
        Tile *** m_puzzle; // stored in row (y) then column (x) order.
        char ** m_images[PUZZLE_NUM_IMAGES];
        void populate_first_image();
        void manipulate_first_image_to_others();
    public:
        Puzzle(int side_len);
        ~Puzzle();
        Tile * get_tile(int x, int y);
        void set_tile(int x, int y, Tile * tile);
        void display();
        void check_solution();
        void populate_images();
        void display_image(int image_id);
        int mark_monsters(int image_id);
        int get_count_of_on_chars(int image_id);
};

class AocDay20 : public AocDay
{
    private:
        static Manipulation m_manipulation_table[TILE_NUM_BORDERS][TILE_NUM_BORDERS]; // first index is from. second index is to.
        void parse_input(string filename, vector<Tile *> & tiles);
        void build_border_lookup_map(vector<Tile *> & tiles, map<int, vector<Tile *>> & lookup_map);
        void classify_tiles(vector<Tile *> & tiles, vector<Tile *> & corners, vector<Tile *> & edges, vector<Tile *> & middles, map<int, vector<Tile *>> & lookup_map);
        void build_manipulation_table();
        void align_for_top_left_corner(Tile * tile, map<int, vector<Tile *>> & border_lookup_map);
        void work_tile_to_right(int row, int col, Puzzle & puzzle, map<int, vector<Tile *>> & border_lookup_map);
        void work_tile_below(int row, int col, Puzzle & puzzle, map<int, vector<Tile *>> & border_lookup_map);
    public:
        AocDay20();
        ~AocDay20();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};

#endif
