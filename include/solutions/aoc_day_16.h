#ifndef __AOC_DAY_16__
#define __AOC_DAY_16__

#include <string>
#include <utility>
#include <regex>

#include "aoc_day.h"

using namespace std;

#define MAX_NUM_FIELDS 20

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

struct Possibilities
{
    int field_position_possibilities[MAX_NUM_FIELDS][MAX_NUM_FIELDS];
    int num_fields;
};

class AocDay16 : public AocDay
{
    private:
        static regex field_regex;
        void parse_input(string filename, Day16Input & input);
        vector<Ticket> remove_invalid_tickets(Day16Input & input);
        void populate_field_position_possibilities(Possibilities & possibilities, vector<Ticket> &valid_tickets, vector<Field> &fields);
        void reduce_field_position_possibilities(Possibilities & possibilities, map<int, int> & fields_to_position_map);
        void display_field_position_possibilities(Possibilities & possibilities, string heading);
        void display_field_position_possibilities(Possibilities & possibilities);
    public:
        AocDay16();
        ~AocDay16();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
