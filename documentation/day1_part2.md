# Day 1, Part 2 #

## Requirements ##

Very similar to Day 1, Part 1.  
Given a set of integer inputs, find the *three* that sum to 2020. Multiply those numbers together and return the result.

### Input Format ###

List of integers, given one integer per line

### Output Format ###

Single integer

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day1_test1.txt) given in the problem statement. Nothing special about it.

### Custom Test Cases ###

None Needed

## Approach ##

Pretty straightforward algorithm with three nested for loops. 

- Read in the list of integers
- For i iterating over the whole list
    - For j iterating from i + 1 to the end of the list
        - For k iterating from j + 1 to the end of the list
            - If i + j + k equals 2020, return i * j * k

## Things I learned ##

The coding of this is very similar to [Day 1, Part 1](day1_part1.md) . However I did find that there are Markdown preview plugins for Notepad++, which will save me many commits fixing up these documents in the future :-)
