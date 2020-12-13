#ifndef __AOC_DAY_10__
#define __AOC_DAY_10__

#include <string>

#include "aoc_day.h"

using namespace std;

class AocDay10 : public AocDay
{
    private:
        vector<long> read_input(string filename);
    public:
        AocDay10();
        ~AocDay10();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
