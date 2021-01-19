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
    
    /*
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
    */
    
    // Cleanup dynamically allocated rules
    for (int i=0; i<rules.size(); i++)
    {
        delete rules[i];
    }
    
    ostringstream out;
    out << matching_messages;
    return out.str();
}

/* Initial plan
Rule 8 can match either:
1) 42
2) 42 followed by 8

Rule 11 can match either:
1) 42 followed by 31
2) 42 followed by 11 followed by 31


Combining these together, we have *m* copies of rule 42, followed by *n* copies of rule 42, followed by *n* copies of rule 31.  
For this, both *m* and *n* must be greater than or equal to 1.

define length_42=length of the rule 42 matches
define length_31=length of the rule 31 matches
Define min_n=1;
Define max_n=(length(message)-length_42)/(length_42 + length_31)

* for (n between min_n and max_n)
    * length_for_m_copies = length(message)-n*(length_42+length_31)
    * if ((length_for_m_copies < length_42) || (length_for_m_copies % length_42 != 0))
        * Invalid value for n - either there aren't enough characters for m=1 to work, or there will be characters leftover
        * Continue to the next n
    * set m = length_for_m_copies / length_42
    * set found = true
    * for (int i=0; i<m+n; i++) // check for the rule 42 matches
        * Create the substring of message for the length_42 characters starting at i*length_42
        * If rule_42 does not have that substring as a possible match
            * set found to false
    * for (int i=0; i<n; i++)
        * Create the substring of messages for the length_31 characters starting at (m+n)*length_42 + i*length_31
        * If rule_31 does not have that substring as a possible match
            * set found to false
    * If found is true, we have found the match and this is one to be added for the output. 
        * We are Done processing. Break out of the loop
*/

bool AocDay19::does_message_match_rules_42_31(string message, Rule * rule42, Rule * rule31)
{
#ifdef DEBUG_DAY19
    cout << " Checking message [" << message << "] against rules 42 and 31" << endl;
#endif
    int length_42=rule42->possible_matches[0].size(); // analysis shows all of these matches are the same length
    int length_31=rule31->possible_matches[0].size();
    
    int min_n=1;
    int max_n=((message.size()-length_42)/(length_42+length_31))+1;
    
#ifdef DEBUG_DAY19
    cout << " The message is length " << message.size() << ". Length of 42 is " << length_42 
         << ". Length of 31 is " << length_31 << ". This means that there can be between 1 and " << max_n << " for the n value" << endl;
#endif
    for (int n=min_n; n<=max_n; n++)
    {
        int length_for_m_copies = message.size()-(n*(length_42+length_31));
#ifdef DEBUG_DAY19
        cout << "  n value of " << n << " results in a length of " << length_for_m_copies << " for copies of m" << endl;
#endif
        if ((length_for_m_copies < length_42) || (length_for_m_copies % length_42 != 0))
        {
#ifdef DEBUG_DAY19
            cout << "   Invalid possible value for length_for_m_copies - either too small or not an even multiple" << endl;
#endif
            continue;
        }
        int m = length_for_m_copies / length_42;
#ifdef DEBUG_DAY19
        cout << "   m value is set to " << m 
             << ". Results in " << m+n << " instances of rule 42 and " << n << " instances of rule 31" << endl;
#endif
        bool found = true;
        for (int i=0; i<m+n; i++)
        {
            string portion = message.substr(i*length_42, length_42);
#ifdef DEBUG_DAY19
            cout << "    Checking [" << portion << "] against rule 42 from characters " << i*length_42 << " to " << (i*length_42)+length_42-1 << endl;
#endif
            bool portion_found = false;
            for (int i=0; i<rule42->possible_matches.size(); i++)
            {
                if (portion == rule42->possible_matches[i])
                {
#ifdef DEBUG_DAY19
                    cout << "     Rule 42 match found" << endl;
#endif
                    portion_found=true;
                    break;
                }
            }
            if (!portion_found)
            {
#ifdef DEBUG_DAY19
                cout << "     No Rule 42 match found. This combination of n and m is invalid" << endl;
#endif
                found=false;
                break;
            }
        }
        if (!found) // short circuit. skip checking the n copies of rule31
        {
            continue;
        }
        for (int i=0; i<n; i++)
        {
            string portion = message.substr((m+n)*length_42 + i*length_31, length_31);
#ifdef DEBUG_DAY19
            cout << "    Checking [" << portion << "] against rule 31 from characters " << (m+n)*length_42 + i*length_31 << " to " << (m+n)*length_42 + i*length_31+length_31-1 << endl;
#endif
            bool portion_found = false;
            for (int i=0; i<rule31->possible_matches.size(); i++)
            {
                if (portion == rule31->possible_matches[i])
                {
#ifdef DEBUG_DAY19
                    cout << "     Rule 31 match found" << endl;
#endif
                    portion_found=true;
                    break;
                }
            }
            if (!portion_found)
            {
#ifdef DEBUG_DAY19
                cout << "     No Rule 31 match found. This combination of n and m is invalid" << endl;
#endif
                found=false;
                break;
            }
        }
        if (found == true)
        {
#ifdef DEBUG_DAY19
            cout << "  Match has been found. Returning true" << endl;
#endif
            return true;
        }
    }
#ifdef DEBUG_DAY19
    cout << "  No match is found. Returnign false" << endl;
#endif
    return false;
}

string AocDay19::part2(string filename, vector<string> extra_args)
{
    vector<Rule *> rules;
    vector<string> messages;
    int matching_messages = 0;
    map<int, Rule *> rule_lookup_map;
    
    parse_input(filename, rules, messages);
    
    clean_up_rules(rules, rule_lookup_map);
    
    build_possible_matches(rules);
    
    Rule * rule0 = rule_lookup_map[0];
    Rule * rule42 = rule_lookup_map[42];
    Rule * rule31 = rule_lookup_map[31];
    
    // time to loop over the messages
    for (int i=0; i<messages.size(); i++)
    {
        cout << "Checking message [" << messages[i] << "]" << endl;
        bool found = false;
        for (int j=0; j<rule0->possible_matches.size(); j++)
        {
            if (messages[i] == rule0->possible_matches[j])
            {
                cout << " Message " << messages[i] << " found in rule 0" << endl;
                found = true;
                break;
            }
        }
        if (!found)
        {
            found = does_message_match_rules_42_31(messages[i], rule42, rule31);
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

