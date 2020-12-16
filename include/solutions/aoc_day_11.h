#ifndef __AOC_DAY_11__
#define __AOC_DAY_11__

#include <regex>

#include "aoc_day.h"

using namespace std;

class AocDay11 : public AocDay
{
    private:
        vector<string> read_input(string filename);
    public:
        AocDay11();
        ~AocDay11();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
