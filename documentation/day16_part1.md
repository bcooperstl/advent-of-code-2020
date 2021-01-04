# Day 16, Part 1 #

## Requirements ##

A field has multiple ranges of valid values. A ticket contains a list of values.

Take the list of nearby tickets, and identify tickets which have values that would not be valid for any field (i.e. they are out of range for all possible field).

Add the list of the invalid values to determine the "ticket scanning error rate" and return the sum of those. There can be multiple invalid values on a ticket, so don't short-circuit this.

### Input Format ###

There is a section of fields that are defined as
`file_name: m-n or mm-nn`, which indicates the valid values for the field

There is then a section called "your ticket:" which contains a comma-separated list of ints for the values for your ticket.

There is then a section called "nearby tickets:" which contain multiple comma-separated lists of ints for the values for nearby tickets.

### Output Format ###

Number - the ticket scanning error rate, as described in the requirements, for all the nearby tickets.

## Test Cases ##

### Given Test Cases ###

There is one provided test case (../data/test_cases/day16_test1.txt).

### Custom Test Cases ###

I will create a second test case (../data/test_cases/day16_test2.txt). This is similar to test 1, but has all of the invalid values on one nearby ticket.

## Approach ##

### Supporting Data Structures ###

There will be a *field* class that will contain:  
* Name
* List of Valid_Value_Ranges. This will be vector<pair<int,int>> to correspond to the valid ranges
* isValid(int) method that will check if a value is valid or not
* Add an error_rate method that takes a Ticket and returns the 

There will be a *ticket* class that will contain:
* List of Values present on the ticket.

There will be an *input* class to contain all of the input:
* vector<field> for the fields
* ticket for your_ticket
* vector<ticket> for nearby tickets

### Parsing the Input File ###

There are three sections to the input file to parse.
1) Fields
2) Your Ticket
3) Nearby Tickets

There are blank lines between the sections.

To parse the fields, loop until the first blank line is reached.  
Use a regex `(\w+): (\d+)-(\d+) or (\d+)-(\d+)` top split out the field name and the ranges. Every element has 2 ranges, so this will work for all of them.

Skip the "your ticket:" label line
Create a FileUtils split_line_to_ints function that works similar to the split_line_to_strings function to read the value lines.

Skip the blank line and the "nearby ticket:" label line.
While there are still lines in the file
    Use the same FileUtils split_line_to_ints function to read each nearby ticket values.

### Main Loop ###

* Parse the intput as described above.
* Initialize the error_rate_sum to 0.
* Create a map<int, bool> valid_values to store my cache.
* Loop over the list of nearby_tickets with ticket
    * For each value in ticket
        * If the value is in the valid_values cache
            * If the cache shows it false (invalid)
                * Add the value to error_rate_sum
            * Continue to the next value
        * Else (the value is not in the cache)
            * Set `found` to false
            * For each field
                * If the value is valid for the field
                    * Set `found` to true
                    * Stop looking at other fields
            * If found is false
                * Add the value to error_rate_sum
            * Store the value and found into the valid_values cache
* Return the error_rate_sum

## Things I learned ##

Another attempt using the c++ regex library.

Optimizing my solution by using a basic cache.
