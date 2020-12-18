# Day 13, Part 1 #

## Requirements ##

Analyze the list of Bus IDs to determine which one will leave next.  
All Buses leave the station at times which are multiples of their Bus ID numbers.  
We are given the first time that we could depart the station, so find the first Bus that will depart after that time.

Return a value that is the ID of the bus multiplied by the minutes we have to wait to board that bus.

### Input Format ###

Two lines of input.
First line: a number corresponding to the earliest timestampt that I could depart a bus
Second line: a comma-separated list of numbers (for Bus ID) or `x` values.

### Output Format ###

Number - the ID of the bus multiplied by the minutes we have to wait to board the bus.

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day13_test1.txt) given.

### Custom Test Cases ###

I don't think I'll need another test case. If I do need one for an edge case, I can make one by modifying the example and setting the start time to match any of the times where a bus appears.

## Approach ##

### Input Parsing ###
* Parse the input using my function for lines based on commas
* The only element in the first line will be the start time
* Return a list of Bus IDs for the second line. Skip the x's for this part

### Main Loop ###
* Parse the input as described above, returning the start_time and a list of Bus_IDs
* Set result_bus_id and result_time variables to 0.
* For each Bus ID:
    * Calculate how many minutes past the last bus the start time is by by doing `start_time mod bus_id`. Store this in `minutes_past_last`.
    * Calculate the next time that bus arrives by `start_time - minutes_past_last + bus_id` and storing that in `next_time`.
    * if `result_bus_id` is `0 or `next_time < result_time`
        * Set `result_bus_id` and `result_time` to this `bus_id` and `next_time` values
* Return `result_bus_id * (result_time - start_time)`

## Things I learned ##

I was trying to think of the most efficient way to do the module arithmetic, and I think this makes the most sense. Find out how many minutes past the bus we are, and then add on how many minutes remain.
