# Day 2, Part 1 #

## Requirements ##

Analyze the list of password lines (See input format) to see which ones contain valid passwords. 
To be valid, a password must contain the required letter within the number of times specified by the range.

### Input Format ###

List of lines from password file. A line contains 4 elements:  
1. Minimum number of occurrences of the required letter
2. Maximum number of occurrences of the required letter
3. The required letter
4. The password

The line format is:

`min-max char: password`

For example:

`1-3 a: abcde`

* 1 is the minimun number of occurrences of the required letter
* 2 is the maximum number of occurrences of the required letter
* a is the required letter
* abcde is the password

### Output Format ###

Integer - the number of passwords that are valid according to that line's policies.

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day2_test1.txt) given in the problem statement. It contains enough edge case information (min and max matching) where I don't think I'll need a custom test case.

### Custom Test Cases ###

None Needed

## Approach ##

Going to create a simple class to store each line from the password file. I will populate the data using a regular expression.

I will add a valid method to this class to check if the password was valid at the time. I can run through the list of lines, counting the number that are valid.

## Things I learned ##

I've done plenty of regular expressions before in shell scripts/Java/Python/etc. but never in c++. This was my first time using the regex library, and it was pretty simple to use.




