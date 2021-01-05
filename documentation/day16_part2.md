# Day 16, Part 2 #

## Requirements ##

A field has multiple ranges of valid values. A ticket contains a list of values.

Take the list of nearby tickets, and remove tickets which have values that would not be valid for any field (i.e. they are out of range for all possible field).

Using the fields and list of valid nearby tickets, determine which field corresponds to which value. Note that the list is constant - field 1 will always be in the same position for all tickets.

When the list of fields has been mapped, generate a solution that is the product of multiplying the values that start with "departure" together.

### Input Format ###

There is a section of fields that are defined as
`file_name: m-n or mm-nn`, which indicates the valid values for the field

There is then a section called "your ticket:" which contains a comma-separated list of ints for the values for your ticket.

There is then a section called "nearby tickets:" which contain multiple comma-separated lists of ints for the values for nearby tickets.

### Output Format ###

Number - the product of the values of the fields that start with "departure"

## Test Cases ##

### Given Test Cases ###

I will be modifying the one provided test case

### Custom Test Cases ###

I modified the [existing sample test case](../data/test_cases/day16_test3.txt) so that two of the three fields start with "departure". This will allow me to fully test the problem's output requirement.

## Approach ##

### Supporting Data Structures ###

There will be a *field* class that will contain:  
* Name
* List of Valid_Value_Ranges. This will be vector<pair<int,int>> to correspond to the valid ranges
* isValid(int) method that will check if a value is valid or not

There will be a *ticket* class that will contain:
* List of Values present on the ticket.

There will be an *input* class to contain all of the input:
* vector<field> for the fields
* ticket for your_ticket
* vector<ticket> for nearby tickets

There will be a `bool[MAX_NUN_FIELDS][MAX_NUM_FIELDS] field_position_possibilities` array. This will be initialized to all be true. As given in the name, the first index is field_id and the second index is position_number.

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

### Removing Invalid Nearby Ticket ###

This copies off of [Part 1](day16_part1.md) in how it will go through the list. I could refactor the common code out, but they behave different enough where there would be a lot of additional code to stub in for both places that the copy/paste is fine enough.
* Create a map<int, bool> valid_values to store my cache.
* Create a vector<Ticket> valid_tickets
* Loop over the list of nearby_tickets with ticket
    * Set a bool ticket_valid to true
    * For each value in ticket
        * If the value is in the valid_values cache
            * If the cache shows it false (invalid)
                * set ticket_valid to false
                * break out of the loop
        * Else (the value is not in the cache)
            * Set `found` to false
            * For each field
                * If the value is valid for the field
                    * Set `found` to true
                    * Stop looking at other fields
            * Store the value and found into the valid_values cache
            * If found is false
                * set ticket_valid to false
                * break out of the loop
    * If ticket_valid is true
        * Add the ticket to the valid_tickets vector
* Return the valid_tickets vector

### Populating the field_position_possibilities array ###

The field_position_possibilities array is a 2-D boolean array that will be true if a field/position combination is possible and false if it is not possible. This will start as all true, and then be set to false based on values in the valid_tickets.

* Loop over the list of valid_tickets with ticket.
    * Loop over each value in ticket with index val_idx
        * Loop over the list of fields with index field_idx
            * If fields[field_idx].is_valid(values[val_idx}) is false (i.e. this value is not valid for this field)
                * Set field_position_possibilities[field_idx][val_idx] to false

After all these loops are done, field_position_possibilities will have true values for only those field/position combinations where all could be valid.

### Reducing the field_position_possibilities array ###

This function will use process-of-elimination to reduce the array, so that there is only one true value in each row/column.

* create boolean arrays fields_done[MAX_NUM_FIELDS] and positions_done[MAX_NUM_FIELDS], with all values set to false.
* clear the map<int, int> fields_to_position_map
* set work_done to false
* Do
    * loop over the fields with field_idx // doing fields first
        * if fields_done[field_idx] is false // haven't solved this field yet
            * count the number of true values in field_position_possibilities[field_idx][n]
            * If the count is 1 // this means the field is solved
                * set fields_done[field_idx] to true
                * store the matching position index for the one true value in position_idx
                * set positions_done[position_idx] to true
                * set all other field_position_possibilities[n][position_idx] to false. // This will make sure it is not counted as a possibility for any other fields
                * set fields_to_position_map[field_idx]=position_idx
                * set work_done to true // we did something; so try again to further refine the solution the next time through
    * loop over the positions with position_idx // doing positions second
        * if positions_done[position_idx] is false // haven't solved this position yet
            * count the number of true values in field_position_possibilities[n][position_idx]
            * If the count is 1 // this means the position is solved
                * set positions_done[position_idx] to true
                * store the matching field index for the one true value in field_idx
                * set fields_done[field_idx] to true
                * set all other field_position_possibilities[field_idx][n] to false. // This will make sure it is not counted as a possibility for any other positions
                * set fields_to_position_map[field_idx]=position_idx
                * set work_done to true // we did something; so try again to further refine the solution the next time through
* while (work_done == true)

### Main Loop ###

* Parse the intput as described above.
* Remove the invalid tickets as described above.
* Create the field_position_possibilities array as all true values
* Create an empty map<int, int> for fields_to_position_map
* Call the function to populate the field_position_possibilities array.
* Call the function to reduce the field_position_possibilities array, also giving it the fields_to_position_map.
* Initialize a product value to 1
* Loop over the list of fields with field_idx.
    * If fields[field_idx] start with "departure"
        * Look up the value position in fields_to_position_map and store in position
        * Multiply the product value by your_ticket.values[position]
* Return the product

## Things I learned ##

Thinking through the design of this algorithm to iteratively go through and match the fields to their positions was a bit complicated. I liked doing all the thinking ahead of time.
