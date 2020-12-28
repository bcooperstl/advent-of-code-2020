# Day 15 #

## Requirements ##

Given a set of starting numbers, calculate the (2020th for part 1|30000000th for part 2) number in the generated sequence following those numbers according to the following rules:  
* If it is the prior number's first time appearing, generate a `0`
* Else, generate the number of times apart since that number of previously stated.

### Input Format ###

The sequence of starting numbers as a comma separated list

### Output Format ###

Integer - Part 1 = The 2020th number in the sequence. Part 2 = The 30000000th number in the sequence.


## Test Cases ##

### Given Test Cases ###

There are 7 Given test cases:
1) `0,3,6` results in `436` for part 1 and `175594` for part 2.
2) `1,3,2` results in `1` for part 1 and `2578` for part 2.
3) `2,1,3` results in `10` for part 1 and `3544142` for part 2.
4) `1,2,3` results in `27` for part 1 and `261214` for part 2.
5) `2,3,1` results in `78` for part 1 and `6895259` for part 2.
6) `3,2,1` results in `438` for part 1 and `18` for part 2.
7) `3,1,2` results in `1836` for part 1 and `362` for part 2.

### Custom Test Cases ###

I do not anticipate any custom test cases needed.

## Approach ##

* Create a map<int, int> `last_turns` that maps a value to its last turn.
* Start a turn counter at 1.
* Initialize a value called prev_number
* Iterate over the starting numbers with value
    * If this is not the last starting number
        * Add/Update the `last_turns` map so that `last_turns[value]=turn_counter`
    * Increment the turn_counter
    * Set the prev_number to value
* While turn_counter <= 2020 for part 1 or <= 30000000 for part 2
    * If `last_turns[prev_number]` does not exist
        * Set last_turns[prev_number] to turn_counter-1
        * Set prev_number to 0
    * Else
        * Calculate new_value = `turn_counter - 1 - last_turns[prev_number]`
        * Set last_turns[prev_number] to turn_counter-1
        * Set prev_number=new_value
    * Increment the turn_counter
* Return prev_number as the solution

### Working through the test example ###

1) *0* is a starting number. Set `last_turns[0]=1`, `prev_number=0`, `turn_counter=2`
2) *3* is a starting number. Set `last_turns[3]=2`, `prev_number=3`, `turn_counter=3`
3) *6* is a starting number. Set `prev_number=6`, `turn_counter=4`
4) *6* is prev_number and not in last_turns. Set `last_turns[6]=4-1=3`, `prev_number=0`, `turn_counter=5`
5) *0* is prev_number and `last_turns[0]=1`. `new_value=5-1-1=3`. `last_turns[0]=5-1=4`. `prev_number=3`, `turn_counter=6`
6) *3* is prev_number and `last_turns[3]=2`. `new_value=6-1-2=3`. `last_turns[3]=6-1=5`. `prev_number=3`, `turn_counter=7`
7) *3* is prev_number and `last_turns[3]=5`. `new_value=7-1-5=1`. `last_turns[3]=7-1=6`. `prev_number=1`, `turn_counter=8`
8) *1* is prev_number and not in last_turns. Set `last_turns[1]=8-1=7`, `prev_number=0`, `turn_counter=9`
9) *0* is prev_number and `last_turns[0]=4`. `new_value=9-1-4=4`. `last_turns[0]=9-1=8`. `prev_number=4`, `turn_counter=10`
10) *4* is prev_number and not in last_turns. Set `last_turns[4]=10-1=9`, `prev_number=0`, `turn_counter=11`

Result of prev_number is *0*, which matches the end of the 10-round sample

## Things I learned ##

For part 1, I thought I could do this brute force with an array of 2020 longs, but I wanted to try something different. I'm glad I did that, since all I had to do was change a constant and I had the part 2 solution.

If this isn't fast enough for part 2, I'll update my map to be a faster data structure.

### First Attempt at optimizing part 2 ###

Part 2 runs in about 55 seconds. I'd like to try to speed this up.
Going to put the values `0-999` in an array, and only use the map for values `1000` or greater.

By going up to 999, I cut the runtime down to around 30 seconds. Going up to 999999 cuts it down to 12 seconds.

