#ifndef __AOC_DAY_2__
#define __AOC_DAY_2__

#include <regex>

#include "aoc_day.h"

using namespace std;

class PasswordFileLine
{
    private:
        static regex line_format;
        long m_min_occurrences;
        long m_max_occurrences;
        long m_matching_positions[2];
        char m_letter;
        string m_password;
    public:
        PasswordFileLine(string input);
        ~PasswordFileLine();
        bool isValidPart1();
        bool isValidPart2();
};

class AocDay2 : public AocDay
{
    private:
        vector<PasswordFileLine> read_input(string filename);
    public:
        AocDay2();
        ~AocDay2();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
