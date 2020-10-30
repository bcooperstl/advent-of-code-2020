#ifndef __AOC_DAY_H__
#define __AOC_DAY_H__

#include <string>
#include <vector>

/* This is the superclass for all of the daily programs.
 * It will have two functions to override - one for part 1, and one for part 2
 * each of those functions takes two arguments
 *  1) the filename for the input file.
    2) a vector of strings for extra arguments. This vector can be empty.
 */

class AocDay
{
    protected:
        int m_day;
    public:
        AocDay(int day);
        ~AocDay();
        virtual std::string part1(std::string filename, std::vector<std::string> extra_args);
        virtual std::string part2(std::string filename, std::vector<std::string> extra_args);
};

#endif
