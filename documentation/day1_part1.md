# Day 1, Part 1 #

## Requirements ##

Given a set of integer inputs, find the two that sum to 2020. Multiply those two numbers together and return the result.

### Input Format ###

List of integers, given one integer per line

### Output Format ###

Single integer

## Test Cases ##

### Given Test Cases ###

There is [one test case](data/test_cases/day1_test1.txt) given in the problem statement. Nothing special about it.

### Custom Test Cases ###

None Needed

## Approach ##

Pretty straightforward algorithm with two nested for loops. 

- Read in the list of integers
- For i iterating over the whole list
    - For j iterating from i + 1 to the end of the list
        - If i + j equals 2020, return i * j

## Things I learned ##

I've been working with iterators more throughout the Advent Of Code template that I created, instead of just using array indices into vectors.

When trying to start j at the i + 1 position, I found the [std::next](https://www.cplusplus.com/reference/iterator/next/) function, which returns an interator to the next element

