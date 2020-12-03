# Day 3, Part 2 #

## Requirements ##

There is a map given, as described in the Input Format section. This map repeats itself left-to-right as many times as needed.

Similar to [part 1](day3_part1.md), we will be traversing the map, starting at the top-left square. There are five different slopes given to follow until you go past the bottom of the map (i.e. include the bottom row).

While traversing the map, count the number of trees (`#` symbols) encountered. Multiply the number of trees for each slope, and return that product.

### Input Format ###

A 2-dimensional rectuanglar map of the area. This map can be read in as a list of strings, with all strings being the same length.

There are two symbols in this map:
- `.` indicates an open space
- `#` indicates a tree

### Output Format ###

An integer - the product of the number of trees encountered on each journey (each of the 5 slopes).

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day3_test1.txt) given. I don't anticipate needing other test cases.

### Custom Test Cases ###

None Needed

## Approach ##

Traversal function, which takes the map, the row_increment and the column_increment as variables:
* Get the height of the area by the number of strings in the map.
* Get the width of the area by the string length of the first string in the map.
* Define my start coordinate as row=0, column=0
* While row is less than height
    * Check the (row,column) position to see if there is a tree or not, and increment the trees counter if a tree is found
    * Increment row by the row_increment.
    * Increment column by the column_increment. Since this could go off the right of the map, use the modulo this by the width to bring it back onto the map. This eliminates the needs to make copies of the map to the right
* Return the number of trees

Main program:
* Read in the input as a list of strings as the map.
* Set the tree product to 1
* For each of the 5 slopes:
    * Call the traversal function with the map, row_increment and column_increment
    * Muliple the tree_product by the result of that function.
* Return the tree product.

### Refactoring Needed ###

Create a new function to do the traversal. Have part1 call that function instead of hardcoding the slope from part 1.

## Things I learned ##

## Things to Consider Doing Differently ##

Instead of passing row_increment and column_increment as integers, I could have created a slope structure and populated those values. I passed them backwards, my first time coding the part 2 solution.
