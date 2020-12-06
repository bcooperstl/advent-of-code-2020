# Day 5, Part 2 #

## Requirements ##

We are given a list of input as specified in the input format. We need to convert these strings to a row with the first 7 characters and a column with the last 3 characters.

A Seat ID is calculated by multiplying the row by 8 and then adding the seat ID.

There is one seat ID missing in the middle of the list that belongs to us. We need to return that Seat ID. Although seat IDs can go from 0 to 1023, they don't start at 0 or end at 1023. We know that ours will not be the first or last seat ID.

### Input Format ###

List of 10-character strings. Each string is as followed:

First 7 characters are either:
* `F` - front half; i.e. a 0
* `B` - back half; i.e. a 1

Last 3 characters are either:
* `L` - left/lower half; i.e. a 0
* `R` - right/upper half; i.e. a 1

### Output Format ###

Integer corresponding to the missing seat ID.

## Test Cases ##

No test cases needed for Part 2, considering how this builds off of [Part 1](day5_part1.md).

## Approach ##

Similar to Part 1, there is a hidden binary encoding.

For each 10 character input, I will split it into the row for characters 1-7 and the column for characters 8-10.
For the row, replace 'F' with '0' and replace 'B' with '1'. Then use strtol with base 2 to convert this to an integer, and that is the row ID.
For the column, replace 'L' with '0' and replace 'R' with '1'. Then use strtol with base 2 to convert this to an integer, and that is the column ID.
Use the formula to create the Seat ID. 

Add all of the Seat IDs to a list.

Sort the list.

Walk through the sorted list, looking for a missing value. The missing value is the Seat ID to return.

## Things I learned ##

Going to use the sort from the <algorithm> header. Done it before, but not in a while.
