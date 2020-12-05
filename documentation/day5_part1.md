# Day 5, Part 1 #

## Requirements ##

We are given a list of input as specified in the input format. We need to convert these strings to a row with the first 7 characters and a column with the last 3 characters.

A Seat ID is calculated by multiplying the row by 8 and then adding the seat ID. We need to find the largest Seat ID from the list.

### Input Format ###

List of 10-character strings. Each string is as followed:

First 7 characters are either:
* `F` - front half; i.e. a 0
* `B` - back half; i.e. a 1

Last 3 characters are either:
* `L` - left/lower half; i.e. a 0
* `R` - right/upper half; i.e. a 1

### Output Format ###

Integer corresponding to the highest seat ID.

## Test Cases ##

### Given Test Cases ###

There are 4 examples given. I will create 5 test cases from them. One for each of them to ensure the calculations are done correctly, then a final test case with all 4 examples to make sure the largest seat ID is selected.

### Custom Test Cases ###

None needed beyond what I am created using the givens.

## Approach ##

This is just a hidden binary encoding.

For each 10 character input, I will split it into the row for characters 1-7 and the column for characters 8-10.
For the row, replace 'F' with '0' and replace 'B' with '1'. Then use strtol with base 2 to convert this to an integer, and that is the row ID.
For the column, replace 'L' with '0' and replace 'R' with '1'. Then use strtol with base 2 to convert this to an integer, and that is the column ID.
Use the formula to create the Seat ID and find the largest Seat ID.

## Things I learned ##

Going to use the std::replace function to replace the characters in the string.

