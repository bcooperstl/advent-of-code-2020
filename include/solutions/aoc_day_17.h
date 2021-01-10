#ifndef __AOC_DAY_17__
#define __AOC_DAY_17__

#include <string>

#include "aoc_day.h"

#define STATE_ACTIVE '#'
#define STATE_INACTIVE '.'

using namespace std;

struct Cell
{
    char curr_state;
    char prev_state;
    int curr_num_neighbors_active;
    int prev_num_neighbors_active;
    char next_state;
};

class Space
{
    private:
        int m_x_size;
        int m_y_size;
        int m_z_size;
        Cell *** m_cells; // this 3-D array is stored as [z][y][x] for ease of display.
        Cell *** allocate_array(int x_size, int y_size, int z_size);
        void deallocate_array(Cell *** array, int x_size, int y_size, int z_size);
        void resize(bool grow_x, bool grow_y, bool grow_z);
    public:
        Space();
        ~Space();
        int get_x_size();
        int get_y_size();
        int get_z_size();
        void run_cycle();
        int count_active();
        void display();
        void set_cell_active(int x, int y, int z);
        void init_from_plane(vector<string> init_plane);
};

class AocDay17 : public AocDay
{
    private:
        vector<string> read_input(string filename);
    public:
        AocDay17();
        ~AocDay17();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
