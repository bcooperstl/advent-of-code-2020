# Day 9, Part 1 #

## Requirements ##

You are given a list of numbers. The preamble is of a given length, which I will call *n*.  
For numbers after the preamble, each number must be a sum of two number in the prior *n* numbers.

Find the first number that does not meet this property and return that value.

### Input Format ###

#### Additional Parameter ####

This program will take an additional parameter which is the length of the preamble. The example test case has a length of 5 and the daily input file has a length of 25.

### Output Format ###

Number - the first value that is not a sum of two of the prior preamble-length numbers.

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day9_test1.txt) given with premable length *5*. I don't anticipate needing other test cases.

### Custom Test Cases ###

None anticipated.

## Approach ##

### Objects needed ###
* Create a struct entry with two members:
    * long value - the current value
    * long prior_sums[24] - up to 24 values for adding this number with the previous up to 24 numbers

### Main Program ###

* Use the existing function to read in the input as a list of strings.

* Allocate an array of `entry` structs with the length of the input

* Loop over the first *preamble_length* entries with `n`:
    * Convert the input entry to a long and store it in the corresponding entry's `value` element
    * Loop backwards from `n-1` to `0`, storing the sum in the `prior_sums` position (i.e. `n`+`n-1` is stored in `prior_sums[0]`; `n`+`n-2` is stored in `prior_sums[1]` etc)

* Continue looping over the non-preamble entries of input with `n`:
    * Convert the input entry to a long and store it in the corresponding entry's `value` element
    * Set found = false
    * Loop from n-24 to n-1 with i
        * Loop from 0 to (i-(n-24)) with j
            * If `entries[i].prior_sums[j]` equals `entry[n].value`
                * Set found = true
                * Break from this loop
        * If found is true
            * Break from this loop
    * If not found
        * Return the not found value
    * Else (it is found)
        * Loop backwards from `n-1` to `n-24`, storing the sum in the `prior_sums` position (i.e. `n`+`n-1` is stored in `prior_sums[0]`; `n`+`n-2` is stored in `prior_sums[1]` etc)

## Things I learned ##

Trying to create an interesting sliding window-of-sums algorithm so that I don't have to recalculate the sums everytime through. I'm sure I'll find some off-by-one errors as I implement it :-)
