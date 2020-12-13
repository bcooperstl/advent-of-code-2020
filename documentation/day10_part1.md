# Day 10, Part 1 #

## Requirements ##

You are given a list of numbers, which correspond to output joltage values. An adapter can take input 1, 2, or 3 jolts lower than the output. Your device has an adapter that is 3 jolts higher than the largest one in the list.

The charging outlet has a joltage of 0 jolts. Daisy-chain all of your joltage adapters together, in order, to step them up. Note that they can be 1, 2, or 3 jolts in difference between adapters.

Count the number of 1, 2, and 3 jolt differences, and return the result of the number of 1 jolt differences multiplied by the number of 3 jolt differences. Include your device's 3 jolt adapter in the list of 3-jolt differences when doing the calculation.

### Input Format ###

A list of integers that correspond to the differnt jolt values of the joltage adapters.

### Output Format ###

Number - the number of 1-jolt step-ups multiplied by the number of 3-jolt step ups.

## Test Cases ##

### Given Test Cases ###

There are two test cases provided. I don't think I'll need other test cases.

1) [Test 1](../data/test_cases/day10_test1.txt)
2) [Test 2](../data/test_cases/day10_test2.txt)

### Custom Test Cases ###

None anticipated.

## Approach ##

### Modification to FileUtils ###

* Add a function that will parse the input and return it as a vector of longs.


### Main Program ###

* Use the new FileUtils function to read in the input as a list of longs.

* Sort that list in ascending order.

* Increment for the first adapter by adding to the difference counter of going from 0 to it. (i.e if the first value is 2, increment the 2 counter)
* Walk the list, calculating the difference between each element and the one before it.
    * Increment counter buckets for the difference.
* Add one to the 3-jolt difference counter bucket to correspond to the device's internal step-up adapter.

* Return the number of 1-jolt differences multiplied by the number of 3-jolt differences.

## Things I learned ##

Going to use the sort method from <algorithm> again for this.
