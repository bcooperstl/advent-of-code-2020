#ifndef __AOC_DAY_19__
#define __AOC_DAY_19__

#include <string>
#include <regex>

#include "aoc_day.h"

using namespace std;

enum RuleType 
{
    literal,
    single_choice,
    multi_choice
};

struct Rule
{
    RuleType type;
    int rule_number;
    int num_choices;
    string literal_value;
    vector<int> child_rule_numbers[2]; // Rule numbers for the child's rules. Can only have one or two children based on data inspection.
    vector<Rule *> child_rules[2]; // pointers to the child's rules.
    vector<string> possible_matches;
    bool has_processed_matches;
};

class AocDay19 : public AocDay
{
    private:
        static regex literal_rule_regex;
        static regex single_choice_rule_regex;
        static regex multi_choice_rule_regex;
        Rule * create_literal_rule(int rule_number, string literal_value);
        Rule * create_single_choice_rule(int rule_number, vector<int> child_rule_numbers);
        Rule * create_multi_choice_rule(int rule_number, vector<int> first_child_rule_numbers, vector<int> second_child_rule_numbers);
        void parse_input(string filename, vector<Rule *> & rules, vector<string> & messages);
        void clean_up_rules(vector<Rule *> & rules, map<int, Rule *> & rule_lookup_map);
    public:
        AocDay19();
        ~AocDay19();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
