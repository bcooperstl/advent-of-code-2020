# Day 14, Part 1 #

## Requirements ##

Simulate a computer assigned values into memory. There is a 3-state-bitmask applied to values as they are assigned. The bitmask and values are 36-bit unsigned integers.

The bitmask contains three values:
1) `X` = the value for this bit is passesd through and not modified.
2) `0` = 0 is always assigned at this bit position
3) `1` = 1 is always assigned at this bit position

The memory is initially set to all 0 values. 

Go through the input file, setting each value as described by the prior bitmask line, given the above conditions.

After all of the input file (format described below) is run, output the sum of all of the value stored in memory.


### Input Format ###

The input is defined as **mask** and **mem** lines.

**Mask** lines are the format `mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X` and contain the bitmask to use for following **mem** lines.

**Mem** lines are the format `mem[address] = value` (i.e. `mem[8] = 11`).

### Output Format ###

Number - the sum of the values in memory.

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day14_test1.txt) given.

### Custom Test Cases ###

I don't think I'll need another test case.

## Approach ##

### Input Parsing ###

I will be using regular expressions to extract the bitmask and address/value data from the lines.

The regular expression to extract the mask is: `^mask = ([01X]{36})$`, so that the bitmask will be extracted with the first group.

The regular expression to use for the mem lines is: `^mem\[(\d+)\] = (\d+\)$`, so that the address is in the first group and the value is in the second group.

### Applying the mask ###

There are two portions to apply the mask - the `1` bits and the `0` bits.

To ensure all `1` bits are set to `1` without modifying anything else, I can OR the value with a number that is a `0` for all `X` maskbits and `1` for all `1` maskbits.

Similary, to ensure all `0` bits are set to `0` without modifying anything else, I can AND the value with a number that is a `1` for all `X` maskbits and `0` for all `0` maskbits.

So `final_value = ((initial_value | one_mask) & zero_mask`

### Data Types ###

I will use the lower 36 bits in uint64_t integers for my masks and values. The higher bits will all be 0s to make the math easier.

### Storing data in memory ###

I will store the data in a map from uint64_t address to uint64_t value. This will allow me to update values easily for when they get overwritten.

### Main Loop ###
* Parse the input into a list of strings.
* Create an empty map for memory.
* Loop over each line
    * If the line matches the mask regex:
        * Extract the mask from the capture group.
        * Calculate the one_mask and zero_mask based on the extracted match.
    * If the line matches the mem regex:
        * Extract the address and value from the capture groups.
        * Apply the current one_mask and zero_mask to the value, and store that result in the memory map for the address.
    * Else
        * Throw an error - did something wrong :-)
* Loop over the map, summing the values and return that sum.

## Things I learned ##

Another chance at using the c++ regex libraries for capture groups. The more I use this, the better I get at it.

If you have a bad regex, you can get a runtime error.

Once again, how to revert a commit to the wrong branch. At least I didn't push this time :-0

