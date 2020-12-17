#ifndef __AOC_DAY_12__
#define __AOC_DAY_12__

#include <string>

#include "aoc_day.h"

using namespace std;

struct Day12Instruction
{
    char action;
    int value;
};

class AocDay12 : public AocDay
{
    private:
        vector<Day12Instruction> parse_input(string filename);
    public:
        AocDay12();
        ~AocDay12();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
