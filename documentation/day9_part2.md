# Day 9, Part 2 #

## Requirements ##

You are given a list of numbers. The preamble is of a given length, which I will call *n*.  
As in [Day 9, Part 1](day9_part1.md), for numbers after the preamble, each number must be a sum of two number in the prior *n* numbers.

There is one number that does not meet this property. First find this number (using part 1).

Then, walk through the list of numbers to find a range of numbers that sums to this value.

The encryption weakness is defined as the largest number in that range plus the smallest number. Return that value.

### Input Format ###

#### Additional Parameter ####

This program will take an additional parameter which is the length of the preamble. The example test case has a length of 5 and the daily input file has a length of 25.

### Output Format ###

Number - encryption weakness as defined in the requirements

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day9_test1.txt) given with premable length *5*. I don't anticipate needing other test cases.

### Custom Test Cases ###

None anticipated.

## Approach ##

This will follow and build on to part 1. Going to take the Part 1 solution copy-and-paste, and then add on to it to do the final calcaultion.

### Objects needed ###
* Create a struct entry with two members:
    * long value - the current value
    * long prior_sums[24] - up to 24 values for adding this number with the previous up to 24 numbers

### Main Program ###

** Copy of part 1**
* Use the existing function to read in the input as a list of strings.

* Allocate an array of `entry` structs with the length of the input

* Compute all of the prior_sums for all elements.
    * Loop over each element from 1 to the end of the list with `i`
        * Loop backwards over the prior `preamble_length - 1` elements of the list with `j`
            * Store the sum of entries[i].value and entries[j].value in entries[i].prior_sums[i-j-1] (i.e. `n`+`n-1` is stored in `prior_sums[0]`; `n`+`n-2` is stored in `prior_sums[1]` etc)

* Start looping over `entries` at `preamble_length` with `n`
    * Set found = false
    * Loop from n-24 to n-1 with i
        * Loop from 0 to (i-(n-24)) with j
            * If `entries[i].prior_sums[j]` equals `entry[n].value`
                * Set found = true
                * Break from this loop
        * If found is true
            * Break from this loop
    * If not found
        * Store this as the not_found value
**Continuing with part 2**

* Initialize some values:
    * running_sum = entries[0].value
    * start_position = 0
    * end_position = 0

Loop to increase the range and adjust accordingly
* While running_sum does not equal the not_found value
    * While running_sum < not_found value
        * Increment end_position and add entries[end_position].value to running_sum
    * While running_sum > not_found value
        * Decrease running_sum by entries[start_position].value
        * Increment start_position

* Loop over entries from start_position to end position to find the largest and smallest value
* Return the sum of those values
        

## Things I learned ##

Trying to create an interesting sliding window for the running_sum calculation.
