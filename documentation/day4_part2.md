# Day 4, Part 2 #

## Requirements ##

Parse the input file into the individual passport records (format described below).

For each passport record, analyze the data to see if it is a valid record or not with the following rules:
* byr, iyr, eyr, hgt, hcl, ecl, pid fields are all *required*
* cid field is *optional*

For each required field, deeper logic rules are required to determine if the provided data is valid. Evaluate each of those rules
against the input data when determinig if a record is valid or not.

Count the number of valid passports and return that number.

### Input Format ###

Series of passport records, separated by newlines.
Each record has up to 8 key:value pairs. Any number of key:value pairs may be on a line. The pairs are sepeated by spaces if they are on the same line.

### Output Format ###

An integer - the number of valid passport records in the input file

## Test Cases ##

### Given Test Cases ###

I created a [test case](../data/test_cases/day4_test2.txt) from the four valid and four invalid examples provided.

### Custom Test Cases ###

I created a [custom test case](../data/test_cases/day4_test3.txt) with all invalid data elements using the last valid example and changing items. Wanted to test that all validators could fail.

## Approach ##

* Same PassportData class, Input Parsing, and Main Program as part 1.

* Going to create an AttributeValidator superclass and 7 child classes to do each validation. Put those in a map based on attribute name.
* When I call the isValid function for part 2, pass in that map of attributes-->AttributeValidators, and allow it to do the deep validation.

## Things I learned ##

Having some fun with polymorphism for the AttributeValidators. I thought this was a better way to go about it than function pointers.

I realized as I was programming that I neglected to originally include a way to map the AttributeValidators to a field, so I needed to add a m_key attribute to them.

This would have taken a bit less time had I just hardcoded a single validation routine, but this was fun this way.

