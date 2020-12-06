# Day 6, Part 2 #

## Requirements ##

You are given a list of questions that people have answered yes to: `a` - `z`. The input is broken down into groups of people as described in the Input format section.  
Combine the yes responses within each group. Calculate how many questions have been answered yes by every member in a group, and sum that up for all the groups.

### Input Format ###

A list of strings.

Each person's answer will be on one line. It will contain one or more characters from `a` to `z`.
There will be 1 or more people per groups. There will be a blank line between groups.

### Output Format ###

Integer - the sum of the yes answers based on group.

## Test Cases ##

### Given Test Cases ###

There are two examples given in the problem that I will use:  
[test 1](../data/test_cases/day6_test1.txt) is the literal example at the bottom.  
[test 2](../data/test_cases/day6_test2.txt) comes from the abcx, abcy, abcz example, and I will use this to verify edge cases.

### Custom Test Cases ###


## Approach ##

This is very similar to part 1, but instead of calculating where 1 person says yes and ORing response together, I need to calculate for all of the group's people saying yes by ANDing responses together.

* Define a person class that has both the string of their input and an integer corresponding to the string.
* Define a group class that has a vector of persons, a function to calculate the group's yes answers, and a function to count the yeses.

Input Parsing:
* Create a new Group
* Loop over the lines of input
    * If a blank line is detected
        * Append the current Group object to the list of groups
        * Create a new Group object for the next entry
    * Else (not a blank line)
        * Create a new Person from the line
        * Add that person to the current Group
* Append the last Group object to the list.

Main Program:  
* Get the list of Groups based on the input
* For each Group:
    * Call the function to get the count of yeses for the group and add that to the total yeses.
* Return the total.

## Refactoring Needed ##

Rename some items from [Part 1](day6_part1.md) to reflect that they correspond to the anyone response, not the everyone response.

## Things I learned or had fun doing ##

I decided that this would be a fun problem for some bit manipulation. It's been a while since I've use the bitwise operators and thought they would be more fun that a bool[26] array.

