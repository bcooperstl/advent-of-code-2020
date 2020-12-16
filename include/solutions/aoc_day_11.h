#ifndef __AOC_DAY_11__
#define __AOC_DAY_11__

#include <regex>

#include "aoc_day.h"
#include "screen.h"

using namespace std;

class AocDay11 : public AocDay
{
    private:
        vector<string> read_input(string filename);
        char find_next_seat(Screen & grid, int start_x, int start_y, int delta_x, int delta_y);
        int count_line_of_sight_occupied_seats(Screen & current, int pos_x, int pos_y);
    public:
        AocDay11();
        ~AocDay11();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
