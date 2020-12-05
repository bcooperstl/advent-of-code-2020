# Day 4, Part 1 #

## Requirements ##

Parse the input file into the individual passport records (format described below).

For each passport record, analyze the data to see if it is a valid record or not with the following rules:
* byr, iyr, eyr, hgt, hcl, ecl, pid fields are all *required*
* cid field is *optional*

Count the number of valid passports and return that number.

### Input Format ###

Series of passport records, separated by newlines.
Each record has up to 8 key:value pairs. Any number of key:value pairs may be on a line. The pairs are sepeated by spaces if they are on the same line.

### Output Format ###

An integer - the number of valid passport records in the input file

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day4_test1.txt) given. I don't anticipate needing other test cases.

### Custom Test Cases ###

None Needed

## Approach ##

* Define a PassportData class that contains a map for attribute and values dentified in the problem description. All fields will be string values, starting as NULL.
* Add an isValid method to this class which checks for the 7 required attributes and returns appropriately.
* Add a setAttribute method that takes the attribute and value, and adds those to the map.

Input Parsing:
* Create a new PassportData
* Loop over the lines of input
    * If a blank line is detected
        * Append the current PassportData object to the list
        * Create a new object for the next entry
    * Else (not a blank line)
        * Set the current position to the start of the line.
        * Loop over the line searching for the first colon and space
        * While a space is found
            * Set the attribute based on the current, colon, and space positions
            * Set the current position to that after the space.
            * Search for next colon and space characters
        * Set the attribute based on the current, colon, and space (end of line) positions
* Append the last PassportData object to the list.

Main Program:

* Parse the input as defined above, resulting in a list of PassportData objects
* Iterate through the list, counting the ones where isValid is true
* return the count

## Things I learned ##

When I created my basic csv parser, I used a simple look-at-each-character method to parse through each line. This program used more 
c++ style find and substr methods instead of examining each character manually.

Also, decided to have some fun and do this with a map instead of 8 individual fields for the attributes.

