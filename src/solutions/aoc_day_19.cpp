#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <set>

#include "aoc_day_19.h"
#include "file_utils.h"

//#define DEBUG_REGEX
#define DEBUG_DAY19

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

/*
Planned approach

From the parsed input
* Add all of the Rule * objects to a map<int, Rule *> based upon the rule_number as the key.
* Loop over all of the rules in the rules_list
    * If the rule is not a literal
        * For each of the choices
            * Iterate over the child_rule_numbers.
                * Find the matching rule from the map, and append that to the appropriate child_rules vector
    * If the rule is a literal
        * append the literal_value in the possible_matches vector. It will be the only element there for literlas
        * Set the has_processed_matches to true
*/

void AocDay19::clean_up_rules(vector<Rule *> & rules, map<int, Rule *> & rule_lookup_map)
{
    for (int i=0; i<rules.size(); i++)
    {
        rule_lookup_map[rules[i]->rule_number]=rules[i];
    }
    
    for (int i=0; i<rules.size(); i++)
    {
        Rule * rule = rules[i];
#ifdef DEBUG_DAY19
        cout << "Cleaning up rule " << rule->rule_number << endl;
#endif
        if (rule->type!=literal)
        {
#ifdef DEBUG_DAY19
            cout << " Non-literal rule with " << rule->num_choices << " choices" << endl;
#endif
            for (int j=0; j<rule->num_choices; j++)
            {
                for (int k=0; k<rule->child_rule_numbers[j].size(); k++)
                {
                    rule->child_rules[j].push_back(rule_lookup_map[rule->child_rule_numbers[j][k]]);
#ifdef DEBUG_DAY19
                    cout << "  Fixing up child " << j << " element " << k << " to point to rule number " << rule->child_rule_numbers[j][k] << ". Points to " << rule->child_rules[j][k]->rule_number << "." << endl;
#endif
                }
            }
        }
        else
        {
#ifdef DEBUG_DAY19
            cout << " Literal rule with value " << rule->literal_value << endl;
#endif
            rule->possible_matches.push_back(rule->literal_value);
            rule->has_processed_matches = true;
        }
    }
    
}

vector<string> AocDay19::build_permutations_from_rule_list(vector<Rule *> rule_list)
{
    vector<string> ret = rule_list[0]->possible_matches;
    for (int i=1; i<rule_list.size(); i++)
    {
        vector<string> next;
        for (int j=0; j<ret.size(); j++)
        {
            for (int k=0; k<rule_list[i]->possible_matches.size(); k++)
            {
                next.push_back(ret[j]+rule_list[i]->possible_matches[k]);
            }
        }
        ret = next;
    }
    return ret;
}

/* Planned approach
This should work, given the statement that there are no loops in our rules.
In the event that this is overwhelming, I'll make a second list of rules based on those that can be reached from 0, and see if that makes it better. If not, I'll need to come up with a new method.

* Initialize a work_done boolean to true.
* do
    * set work_done to false
    * Loop over every rule in the rule list with rule
        * if rule.has_processed_matches is true, then continue; it's already done
        * Loop over all of the elements in both child_rules vectors
            * If all of them have has_processed_matches set to true
                * Make the list of possible_matches by combining those from the child rules appropriately
                * set this rule's has_processed_matches to true
                * Set work_done to true so that we know we did something
* while work_done is true
*/

void AocDay19::build_possible_matches(vector<Rule *> & rules)
{
    bool work_done = true;
    do
    {
#ifdef DEBUG_DAY19
        cout << "New Possible Matches Iteration" << endl;
#endif
        work_done = false;
        for (int i=0; i<rules.size(); i++)
        {
            Rule * rule = rules[i];
            if (rule->has_processed_matches == true)
            {
                continue; // already done
            }
#ifdef DEBUG_DAY19
            cout << " Checking non-processed rule " << rule->rule_number << endl;
#endif
            bool all_children_processed = true;
            for (int j=0; j<rule->num_choices; j++)
            {
                for (int k=0; k<rule->child_rules[j].size(); k++)
                {
                    if (rule->child_rules[j][k]->has_processed_matches == false)
                    {
#ifdef DEBUG_DAY19
                        cout << "  Cannot process because child rule " << rule->child_rule_numbers[j][k] << " is not yet processed" << endl;
#endif
                        all_children_processed = false;
                    }
                }
            }
            if (all_children_processed == false)
            {
                continue;
            }
            for (int j=0; j<rule->num_choices; j++)
            {
                vector<string> permutations = build_permutations_from_rule_list(rule->child_rules[j]);
#ifdef DEBUG_DAY19_VERBOSE
                cout << "  Child " << j << " contains the possible permutations:" << endl;
                for (int k=0; k<permutations.size(); k++)
                {
                    cout << "   " << permutations[k] << endl;
                }
#endif
                rule->possible_matches.insert(rule->possible_matches.end(), permutations.begin(), permutations.end());
            }
#ifdef DEBUG_DAY19
            cout << "  Marking rule " << rule->rule_number << " has_processed_matches to true" << endl;
#endif
            rule->has_processed_matches = true;
            work_done = true;
        }
    } while (work_done == true);
    
    
}

string AocDay19::part1(string filename, vector<string> extra_args)
{
    vector<Rule *> rules;
    vector<string> messages;
    int matching_messages = 0;
    map<int, Rule *> rule_lookup_map;
    
    parse_input(filename, rules, messages);
    
    clean_up_rules(rules, rule_lookup_map);
    
    build_possible_matches(rules);
    
    Rule * rule0 = rule_lookup_map[0];
    
    // time to loop over the messages
    for (int i=0; i<messages.size(); i++)
    {
        bool found = false;
        for (int j=0; j<rule0->possible_matches.size(); j++)
        {
            if (messages[i] == rule0->possible_matches[j])
            {
                cout << "Message " << messages[i] << " found in rule 0" << endl;
                found = true;
                break;
            }
        }
        if (found == true)
        {
            cout << " Incrementing matching_message counter" << endl;
            matching_messages++;
        }
        else
        {
            cout << "Message " << messages[i] << " not found" << endl;
        }
    }
    
    // Analysis for part 2
    for (int i=0; i<rules.size(); i++)
    {
        set<int> sizes;
        for (int j=0; j<rules[i]->possible_matches.size(); j++)
        {
            sizes.insert(rules[i]->possible_matches[j].size());
        }
        cout << "Rule " << rules[i]->rule_number << " has matches of " << sizes.size() << " size:";
        for (set<int>::iterator pos = sizes.begin(); pos != sizes.end(); ++pos)
        {
            cout << " " << *pos;
        }
        cout << endl;
    }
    
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
    rule->type=single_choice;
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
    rule->type=multi_choice;
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

