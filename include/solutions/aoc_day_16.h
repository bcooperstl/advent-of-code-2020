#ifndef __AOC_DAY_16__
#define __AOC_DAY_16__

#include <string>
#include <utility>
#include <regex>

#include "aoc_day.h"

using namespace std;

class Field
{
    private:
        string m_name;
        vector<pair<int, int>> m_valid_value_ranges;
    public:
        Field(string name);
        ~Field();
        void add_valid_value_range(int low, int high);
        string get_name();
        bool is_valid(int target);
};

struct Ticket
{
    vector<int> values;
};

struct Day16Input
{
    vector<Field> fields;
    Ticket your_ticket;
    vector<Ticket> nearby_tickets;
};

class AocDay16 : public AocDay
{
    private:
        static regex field_regex;
        void parse_input(string filename, Day16Input & input);
    public:
        AocDay16();
        ~AocDay16();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
