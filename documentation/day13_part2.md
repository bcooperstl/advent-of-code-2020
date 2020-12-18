# Day 13, Part 2 #

## Requirements ##

Analyze the list of Bus IDs to determine when they can fit into a schedule so that bus_id 0 happens at time 0, bus_id 1 happens at time 1, etc. When there are x values in the list of IDs, it doesn't matter what happens at those times.

Return the value of the minimum timestamp for that the schedule holds true. It will be over a trillion.

### Input Format ###

Two lines of input.
First line: a number corresponding to the earliest timestampt that I could depart a bus. Ignore that for Part 2
Second line: a comma-separated list of numbers (for Bus ID) or `x` values for that time spot doesn't matter.

### Output Format ###

Number - the ID of the bus multiplied by the minutes we have to wait to board the bus.

## Test Cases ##

### Given Test Cases ###

There are five test cases given:
* [1](../data/test_cases/day13_test1.txt)
* [2](../data/test_cases/day13_test2.txt)
* [3](../data/test_cases/day13_test3.txt)
* [4](../data/test_cases/day13_test4.txt)
* [5](../data/test_cases/day13_test5.txt)

### Custom Test Cases ###

Hoping I don't need to make up a custom test case.

## Approach ##

I will be doing this working through the Chinese Remainder Theorem. I found a good [Youtube Video](https://www.youtube.com/watch?v=2-tdwLqyaKo) which explains it straightfoward, so I will be working on implementing that algorithm.

### Input Parsing ###
* Parse the input using my function for lines based on commas
* Skip the first line
* Return a list of Bus IDs for the second line, with 0's in place of where the x's are.

### Main Loop ###
* Parse the input as described above, returning the list of Bus_IDs
*** CONTINUE HERE ***


## Things I learned ##

Application and implementation of a Chinese Remainder Theorem algorithm.