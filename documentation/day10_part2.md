# Day 10, Part 2 #

## Requirements ##

You are given a list of numbers, which correspond to output joltage values. An adapter can take input 1, 2, or 3 jolts lower than the output. Your device has an adapter that is 3 jolts higher than the largest one in the list.

The charging outlet has a joltage of 0 jolts. Daisy-chain all of your joltage adapters together, in order, to step them up. Note that they can be 1, 2, or 3 jolts in difference between adapters.

We need to calculate the number of different combinations of ways to connect the adapters. This will be a large number - over a trillion.

### Input Format ###

A list of integers that correspond to the differnt jolt values of the joltage adapters.

### Output Format ###

Number - the number of different ways to connect the adapters.

## Test Cases ##

### Given Test Cases ###

There are two test cases provided.

1) [Test 1](../data/test_cases/day10_test1.txt)
2) [Test 2](../data/test_cases/day10_test2.txt)

### Custom Test Cases ###

 I anticpate having to make more, but I'm not sure yet.

## Approach ##

### Main Program ###

* Use the new FileUtils function to read in the input as a list of longs.
* Add the value 0 to that list to correspond to the input
* Sort that list in ascending order.

* Start a combination counter at 1. This will be a multiplied value
* Partition the vector into a set of vectors, splitting on cases where the difference between two neighboring values is 3.
* Walk through those partitions
    * Determine the number of combinations in each partition
    * Multiple the combination_counter by that value
* Return the final combination counter.

### Calculating the number of combinations in each partition ###

* First I took the simple case - a partition of 1 or 2 elements can only have 1 combination. Return that.
* Based on how I made my partitions, I know that each combination must include the first and last elements.
* Permute all of the other combinations; checking each of the 2^(length-2) combinations.
* I used a binary number to correspond to which elements were included in each of those combinations that I worked.
* Walking through the options from 0 to 2^(length-2), count those positions with a 1-bit as included and those with a 0-bit as not included
    * See if it is a valid combination - all included elements are 3 or less than the previously included element


## Things I learned ##

This seems like an interesting algorithm I came up with to calculate the valid combinations, using the binary digits to correspond to which one was on.
