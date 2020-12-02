# Day 2, Part 1 #

## Requirements ##

Similar to Day 2, Part 1 - Analyze the list of password lines (See input format) to see which ones contain valid passwords. 
For part 2, to be valid, a password must contain the required letter at *exactly one* of the two positions indicated by the two numbers given.

### Input Format ###

List of lines from password file. A line contains 4 elements:  
1. Position 1 to check
2. Position 2 to check
3. The required letter
4. The password

The line format is:

`pos1-pos2 char: password`

For example:

`1-3 a: abcde`

* 1 is the first position that could contain the letter
* 2 is the second position that could contain the letter
* a is the required letter
* abcde is the password

### Output Format ###

Integer - the number of passwords that are valid according to that line's policies.

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day2_test1.txt) given in the problem statement. It contains enough test case information (0, 1, and 2 matching characters)

### Custom Test Cases ###

None Needed

## Approach ##

Going to use the same class as in [Day 2, Part 1](day2_part1.md) to store the password line data. I can use the same regular expression as part 1, just assign the values to new variables and have another isValid method for the new logic.

### Refactoring Needed ###

Renaming the isValid method to isValidPart1, since part 2 has a different validation routine.

## Things I learned ##

The regular expression library in c++ was the main new learning for both of today's parts.

I did try to see if c++ has a logic XOR operator. It does not - only bitwise. That would have saved a couple lines.
