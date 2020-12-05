#ifndef __AOC_DAY_5__
#define __AOC_DAY_5__

#include "aoc_day.h"

class AocDay5 : public AocDay
{
    private:
        vector<string> read_input(string filename);
        int calculate_row(string boarding_pass);
        int calculate_column(string boaring_pass);
        int calculate_seat_id(int row, int column);
    public:
        AocDay5();
        ~AocDay5();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
