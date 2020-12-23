# Day 14, Part 2 #

## Requirements ##

Simulate a computer assigned values into memory. There is a 3-state-bitmask applied to addresses as values are assigned. The bitmask and values are 36-bit unsigned integers.

The bitmask contains three values:
1) `0` = The corresponding memory address bit is not changed
2) `1` = The corresponding memory address bit is set to `1`
3) `X` = The corresponding memory address bit is **floating** - meaning it can be any and all values.

The memory is initially set to all 0 values. 

Go through the input file, setting each value as described by the prior bitmask line, given the above conditions.

After all of the input file (format described below) is run, output the sum of all of the value stored in memory.

### Data Analysis ###

There are at most 9 `X` values in a mask line, which means there are at most 2^9 = 512 possible values to be set on a mem line. This is doable for a brute force map.

### Input Format ###

The input is defined as **mask** and **mem** lines.

**Mask** lines are the format `mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X` and contain the bitmask to use for following **mem** lines.

**Mem** lines are the format `mem[address] = value` (i.e. `mem[8] = 11`).

### Output Format ###

Number - the sum of the values in memory.

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day14_test2.txt) given. It is different than the Day 14 Part 1 test case.

### Custom Test Cases ###

I don't think I'll need another test case.

## Approach ##

### Input Parsing ###

I will be using regular expressions to extract the bitmask and address/value data from the lines. These are the same regular expressions as [Day 14 Part 1](day14_part1.md). 

The regular expression to extract the mask is: `^mask = ([01X]{36})$`, so that the bitmask will be extracted with the first group.

The regular expression to use for the mem lines is: `^mem\[(\d+)\] = (\d+\)$`, so that the address is in the first group and the value is in the second group.

### Creating the list of masks to apply ###

When a new mask value is given, we will create several masks.

The first one is a set_ones_mask that will be used to set all of the `1` bits to 1. It will consist of `1` values in all of those bit positions, and `0` values for the `0` and `X` bits.

The second one is a clear_x_mask that will be used to set all of the `X` bits to 0. It will consist of `0` values in all of those bit positions, and `1` values for the `0` and `1` bits.

There will then be a list of x_masks created to apply the different X permutations.

* Create a list of x_masks that initially the value 0. This list will contain all of the values to OR the base_address against.
* For every `X` bit in the mask
    * For every item in the x_masks list
        * Append a new copy of that item, with the position of the current `X` bit set to `1`. This will double the size of the list each time.

### Generating the list of addresses ###

There is a multistep process to get all of the addresses from the base address and the mask.

* Perform `base_address` OR `set_ones_mask` to set all of the ones bits and store in `base_address`
* Perform `base_address` AND `clear_x_mask` to set all of the bits for the X's to 0. Store in `base_address`.
* Loop over `x_masks` list with `x_mask`
    * Append `base_address` OR `x_mask` and add that to the addresses list.
* Return the addresses list.

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
        * Create the set_ones_mask, clear_x_mask, and x_masks list from this mask.
    * If the line matches the mem regex:
        * Extract the address and value from the capture groups.
        * Generate the list of addresses from the base_address and current set of masks.
        * Store the value for each of the addresses in the list.
    * Else
        * Throw an error - did something wrong :-)
* Loop over the map, summing the values and return that sum.

## Things I learned ##

Thinking through the design of this was a bunch of the learning. Figuring out the best way to calculate all of the different masks I needed to find all the addresses.

Looked through the data ahead of time to see if it would be able to be forced into a map, or if the amount of entries would be so big that I'd need a different data structure.
