#ifndef __AOC_DAY_6__
#define __AOC_DAY_6__

#include "aoc_day.h"

#include <cstdint>

using namespace std;

class Person
{
    private:
        string m_response;
        uint32_t m_yeses;
    public:
        Person(string response);
        ~Person();
        string get_response();
        uint32_t get_yeses();
};

class Group
{
    private:
        vector<Person *> m_persons;
        uint32_t m_group_any_yeses;
        uint32_t m_group_all_yeses;
    public:
        Group();
        ~Group();
        void add_person(Person * person);
        uint32_t get_group_any_yeses();
        uint32_t get_group_all_yeses();
        int get_any_yes_count();
        int get_all_yes_count();
};

class AocDay6 : public AocDay
{
    private:
        vector<Group *> parse_input(string filename);
    public:
        AocDay6();
        ~AocDay6();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
