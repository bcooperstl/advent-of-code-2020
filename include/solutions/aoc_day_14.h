#ifndef __AOC_DAY_14__
#define __AOC_DAY_14__

#include <regex>
#include <string>
#include <map>

#include "aoc_day.h"

using namespace std;

class AocDay14 : public AocDay
{
    private:
        static regex mask_regex;
        static regex mem_regex;
        
        vector<string> read_input(string filename);
        void setup_mask_values(string mask_str, uint64_t & zero_mask, uint64_t & ones_mask);
    public:
        AocDay14();
        ~AocDay14();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
