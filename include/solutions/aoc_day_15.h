#ifndef __AOC_DAY_15__
#define __AOC_DAY_15__

#include "aoc_day.h"

class AocDay15 : public AocDay
{
    private:
        vector<long> read_input(string filename);
    public:
        AocDay15();
        ~AocDay15();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
