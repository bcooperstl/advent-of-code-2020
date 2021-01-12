#ifndef __AOC_DAY_18__
#define __AOC_DAY_18__

#include "aoc_day.h"

#define OPERATION_ASSIGN 0
#define OPERATION_ADD 1
#define OPERATION_MULTIPLY 2

class AocDay18 : public AocDay
{
    private:
        vector<string> read_input(string filename);
        long eval_expression(string::iterator & pos, string::iterator & end, int level);
    public:
        AocDay18();
        ~AocDay18();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
