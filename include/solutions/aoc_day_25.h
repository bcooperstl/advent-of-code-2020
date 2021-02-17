#ifndef __AOC_DAY_25__
#define __AOC_DAY_25__

#include "aoc_day.h"

class AocDay25 : public AocDay
{
    private:
        vector<long> read_input(string filename);
    public:
        AocDay25();
        ~AocDay25();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
