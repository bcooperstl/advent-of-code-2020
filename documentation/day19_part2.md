# Day 19, Part 2 #

## Requirements ##

Given a grammar with a set of rules, I need to determine which messages are valid, starting with rule 0.

A valid message follows the rules, and does not leave any extra data left over.

This grammar is slightly modified from [Part 1](day19_part1.md) so that there are a couple rules that do contain loops.

1) Rule 8 can match
    a) 42
    b) 42 followed by 8
2) Rule 11 can match
    a) 42 followed by 31
    b) 42 followed by 11 followed by 31

This increases complexity significantly.

### Format of Rules ###

There are several different types of rules:

#### Literal Rules ####

`4: "a"` is a literal rule.  
That means that for rule 4, the literal `a` is the value in the message.

#### Single Choice Rules ####

`0: 4 1 5` is a single choice rule.  
This means that for rule 0, there is rule 4, followed by rule 1, followed by rule 5.

#### Multiple Choice Rules ####

`1: 2 3 | 3 2` is a multiple choice rule.  
This means that for rule run, there is either rule 2 followed by rule 3 or rule 3 followed by rule 2.

By inspection of the data, we know that there will be at most 2 choices for an element

### Input Format ###

List of Rules followed by a blank line, followed by a list of messages to test against the rules.

### Output Format ###

Integer -  the number of messages that match rule 0.

## Test Cases ##

### Given Test Cases ###

There is [one](../data/test_cases/day19_test2.txt) given test case, with the same rule changes as the problem statement.

### Custom Test Cases ###

I don't anticipate needing a custom test case.

## Approach ##

### Analysis ###

I am not going to do a context free grammar, because I think the changes are simple enough given the full enumerations that I did from part 1.

I am going to start by processing the same as part 1, to see if it matches Rule 0 as before.

If it does not match, I'll have to check to see if it matches based on expanded rule 8 or based on expanded rule 11. See below for that logic.

I need to do some quick analysis to see if all of the matches are the same length for rules 8, 11, 42, and 31. If these are different lengths, that makes this more complicated.
**Analysis Done. All rules have matches of the same size**

### Data Structures ###

Define a rule_type with values LITERAL, SINGLE, and MULTIPLE

Create a rule structure with the following
* rule_type
* rule_number
* num_choices
* literal_value
* vector<int> child_rule_numbers[2]. This contains the numbers for the child rules
* vector<Rule *> child_rules[2]. This contains the pointers to the child rules
* vector<string> possible_matches. This contains all possible matches including this level's input.
* has_processed_matches boolean; initially set to false.

### Parsing the Data ###

I will use the following three regular expressions to parse the rules:

1) Identify a literal rule: `^(\d+): \"(.+)\"$`. This extracts the rule number in element 1 and the literal in element 2.
2) Identify a single-choice rule: `^(\d+): ([\d ]+)$`. This extracts the rule number in element 1, and the list of child rules in element 2.
3) Identify a multi-choice rule: `^(\d+): ([\d ]+) \| ([\d ]+)$`. This extracts the rule number in element 1, and the first list of child rules in element 2, and the second list of child rules in element 3.

For both the single-choice and multi-choice rules, I can then use the existing FileUtiil's split_line_to_ints function to split this up.

This portion will return a list of Rule objects with the type, rule_number, num_choices, and child_rule_numbers filled in. The child_rules will still be empty.

When a blank line is found, I know anything after it is messages, and I can read them into a separate list.

### Cleaning up the Data ###

From the parsed input
* Add all of the Rule * objects to a map<int, Rule *> based upon the rule_number as the key.
* Loop over all of the rules in the rules_list
    * If the rulis not a literal
        * For each of the choices
            * Iterate over the child_rule_numbers.
                * Find the matching rule from the map, and append that to the appropriate child_rules vector
    * If the rule is a literal
        * append the literal_value in the possible_matches vector. It will be the only element there for literlas
        * Set the has_processed_matches to true

### Identifying all possible matches ###

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

### Matching a message against the Rule 0 ###

Since we have already identified all of the possible matches for rule 0, just see if the input is in that list.

### Matching a message against Rule 8/11 expansions ###

Rule 0 is defined as rule 8 followed by rule 11.  

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
    * set m = length_for_m_copies % length_42
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

## Things I learned ##

The data analysis and developing the algorithm was fun to do. Interesting ways to only do this for the two rules that needed extra checking.

### Future Work ###

Maybe work on doing this as an implenetation of a Context Free Gammar? Maybe with flex/bison



