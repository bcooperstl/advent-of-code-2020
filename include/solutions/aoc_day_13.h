#ifndef __AOC_DAY_13__
#define __AOC_DAY_13__

#include <string>

#include "aoc_day.h"

using namespace std;

class AocDay13 : public AocDay
{
    private:
        void parse_input_part_1(string filename, long & timestamp, vector<long> & bus_ids );
    public:
        AocDay13();
        ~AocDay13();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
