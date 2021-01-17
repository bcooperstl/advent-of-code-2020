#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>

#include "aoc_day_19.h"
#include "file_utils.h"

#define DEBUG_REGEX
//#define DEBUG_DAY19

using namespace std;

regex AocDay19::literal_rule_regex("^(\\d+): \"(.+)\"$");
regex AocDay19::single_choice_rule_regex("^(\\d+): ([\\d ]+)$");
regex AocDay19::multi_choice_rule_regex("^(\\d+): ([\\d ]+) \\| ([\\d ]+)$");

AocDay19::AocDay19():AocDay(19)
{
}

AocDay19::~AocDay19()
{
}

/* Planned approach to input parsing:
I will use the following three regular expressions to parse the rules:

1) Identify a literal rule: `^(\d+): \"(.+)\"$`. This extracts the rule number in element 1 and the literal in element 2.
2) Identify a single-choice rule: `^(\d+): (\d[\d ]+)$`. This extracts the rule number in element 1, and the list of child rules in element 2.
3) Identify a multi-choice rule: `^(\d+): (\d[\d ]+) | (\d[\d ]+)$`. This extracts the rule number in element 1, and the first list of child rules in element 2, and the second list of child rules in element 3.

For both the single-choice and multi-choice rules, I can then use the existing FileUtiil's split_line_to_ints function to split this up.

This portion will return a list of Rule objects with the type, rule_number, num_choices, and child_rule_numbers filled in. The child_rules will still be empty.

When a blank line is found, I know anything after it is messages, and I can read them into a separate list.
*/

void AocDay19::parse_input(string filename, vector<Rule *> & rules, vector<string> & messages)
{
    rules.clear();
    messages.clear();
    
    FileUtils fileutils;
    vector<string> lines;
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
    
    vector<string>::iterator line_pos = lines.begin();
    // section 1 - fields
    while (*line_pos != "")
    {
        string line = *line_pos;
        ++line_pos; //increment to the next line so that when I do a continue I don't have to manually advance it in 4 separate places
#ifdef DEBUG_REGEX
        cout << "Field Regex - Processing line [" << line << "]" << endl;
#endif
        match_results<std::string::const_iterator> matches;
    
        // Check literal rule first
        regex_search(line, matches, literal_rule_regex);
        if (matches.size() > 0)
        {
#ifdef DEBUG_REGEX
            cout << " Literal Rule Regex matches for rule_id " << matches[1] << " expression [" << matches[2] << "]" << endl;
#endif
            rules.push_back(create_literal_rule(strtol(matches[1].str().c_str(), NULL, 10), matches[2].str()));
            continue;
        }
        
        // Check single choice rule second
        regex_search(line, matches, single_choice_rule_regex);
        if (matches.size() > 0)
        {
#ifdef DEBUG_REGEX
        cout << " Single Choice Rule Regex matches for rule_id " << matches[1] << " child_rules [" << matches[2] << "]" << endl;
#endif
            int rule_number = strtol(matches[1].str().c_str(), NULL, 10);
            vector<int> child_rule_numbers = fileutils.split_line_to_ints(matches[2].str(), ' ');
            
            rules.push_back(create_single_choice_rule(rule_number, child_rule_numbers));
            continue;
        }

        // Check single choice rule second
        regex_search(line, matches, multi_choice_rule_regex);
        if (matches.size() > 0)
        {
#ifdef DEBUG_REGEX
        cout << " Multi Choice Rule Regex matches for rule_id " << matches[1] << " child_rules_1 [" << matches[2] << "] child_rules_2 [" << matches[3] << "]" << endl;
#endif
            int rule_number = strtol(matches[1].str().c_str(), NULL, 10);
            vector<int> child_rule_numbers_1 = fileutils.split_line_to_ints(matches[2].str(), ' ');
            vector<int> child_rule_numbers_2 = fileutils.split_line_to_ints(matches[3].str(), ' ');
            
            rules.push_back(create_multi_choice_rule(rule_number, child_rule_numbers_1, child_rule_numbers_2));
            continue;
        }

        else
        {
            // Not a mask line. Search to see if it is a mem line
#ifdef DEBUG_REGEX
            cout << " *****No Regex matches! Fix your logic!!" << endl;
#endif
        }
    }
    
    // At the blank line. Increment once to skip it.
    ++line_pos;
    
    while (line_pos != lines.end())
    {
        messages.push_back(*line_pos);
        ++line_pos;
    }
    return;
}


/* planned approach
* Parse the intput as described above.
* Initialize the error_rate_sum to 0.
* Create a map<int, bool> valid_values to store my cache.
* Loop over the list of nearby_tickets with ticket
    * For each value in ticket
        * If the value is in the valid_values cache
            * If the cache shows it false (invalid)
                * Add the value to error_rate_sum
            * Continue to the next value
        * Else (the value is not in the cache)
            * Set `found` to false
            * For each field
                * If the value is valid for the field
                    * Set `found` to true
                    * Stop looking at other fields
            * If found is false
                * Add the value to error_rate_sum
            * Store the value and found into the valid_values cache
* Return the error_rate_sum

*/

string AocDay19::part1(string filename, vector<string> extra_args)
{
    vector<Rule *> rules;
    vector<string> messages;
    int matching_messages = 0;
    
    parse_input(filename, rules, messages);
    
    // Cleanup dynamically allocated rules
    for (int i=0; i<rules.size(); i++)
    {
        delete rules[i];
    }
    
    ostringstream out;
    out << matching_messages;
    return out.str();
}

Rule * AocDay19::create_literal_rule(int rule_number, string literal_value)
{
    Rule * rule = new Rule();
    rule->type=literal;
    rule->rule_number=rule_number;
    rule->num_choices=0;
    rule->literal_value=literal_value;
    rule->child_rule_numbers[0].clear();
    rule->child_rule_numbers[1].clear();
    rule->child_rules[0].clear();
    rule->child_rules[1].clear();
    rule->possible_matches.clear();
    rule->has_processed_matches=false;
    
    return rule;
}

Rule * AocDay19::create_single_choice_rule(int rule_number, vector<int> child_rule_numbers)
{
    Rule * rule = new Rule();
    rule->type=literal;
    rule->rule_number=rule_number;
    rule->num_choices=1;
    rule->literal_value="";
    rule->child_rule_numbers[0]=child_rule_numbers;
    rule->child_rule_numbers[1].clear();
    rule->child_rules[0].clear();
    rule->child_rules[1].clear();
    rule->possible_matches.clear();
    rule->has_processed_matches=false;
    
    return rule;
}

Rule * AocDay19::create_multi_choice_rule(int rule_number, vector<int> first_child_rule_numbers, vector<int> second_child_rule_numbers)
{
    Rule * rule = new Rule();
    rule->type=literal;
    rule->rule_number=rule_number;
    rule->num_choices=2;
    rule->literal_value="";
    rule->child_rule_numbers[0]=first_child_rule_numbers;
    rule->child_rule_numbers[1]=second_child_rule_numbers;
    rule->child_rules[0].clear();
    rule->child_rules[1].clear();
    rule->possible_matches.clear();
    rule->has_processed_matches=false;
    
    return rule;
}

