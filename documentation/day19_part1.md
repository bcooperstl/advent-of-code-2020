# Day 19, Part 1 #

## Requirements ##

Given a grammar with a set of rules, I need to determine which messages are valid, starting with rule 0.

A valid message follows the rules, and does not leave any extra data left over.

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

There is [one](../data/test_cases/day19_test1.txt) given test case.

### Custom Test Cases ###

I have a feeling that I will need something more, but I haven't identified it yet.

## Approach ##

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

I don't know if this will work or this will scale.


## Things I learned ##

More fun with c++ regular expressions.
Fun determining an algorithm to evaluate if the contents match or not.

