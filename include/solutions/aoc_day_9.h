#ifndef __AOC_DAY_9__
#define __AOC_DAY_9__

#include <string>

#include "aoc_day.h"

#define MAX_PREAMBLE_LENGTH 25

using namespace std;

struct Entry
{
    long value;
    long prior_sums[MAX_PREAMBLE_LENGTH-1];
};

class AocDay9 : public AocDay
{
    private:
        vector<string> read_input(string filename);
        void compute_prior_sums(vector<Entry> & entries, int preamble_length);
    public:
        AocDay9();
        ~AocDay9();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
