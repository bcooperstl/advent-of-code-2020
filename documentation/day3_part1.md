# Day 3, Part 1 #

## Requirements ##

There is a map given, as described in the Input Format section. This map repeats itself left-to-right as many times as needed.

Traverse the map, starting at the top-left square with the slope of right 3, down 1. Continue until you go past the bottom of the map (i.e. include the bottom row).

While traversing the map, count the number of trees (`#` symbols) encountered, and return that total number of trees.

### Input Format ###

A 2-dimensional rectuanglar map of the area. This map can be read in as a list of strings, with all strings being the same length.

There are two symbols in this map:
- `.` indicates an open space
- `#` indicates a tree

### Output Format ###

An integer - the number of tree encountered on the journey.

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day3_test1.txt) given. I don't anticipate needing other test cases.

### Custom Test Cases ###

None Needed

## Approach ##

* Read in the input as a list of strings as the map.
* Get the height of the area by the number of strings in the map.
* Get the width of the area by the string length of the first string in the map.
* Define my start coordinate as row=0, column=0
* While row is less than height
    * Check the (row,column) position to see if there is a tree or not, and increment the trees counter if a tree is found
    * Increment row by 1 to move down 1
    * Increment column by 3 to move right 3. Since this could go off the right of the map, use the modulo this by the width to bring it back onto the map. This eliminates the needs to make copies of the map to the right
* Return the number of trees

## Things I learned ##


