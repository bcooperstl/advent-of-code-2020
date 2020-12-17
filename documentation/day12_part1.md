# Day 12, Part 1 #

## Requirements ##

Given a set of actions and values, move the ship around the plane as specified. For our sake, the ship starts at the origin, facing East.

There are three types of actions:
1) Move a specified direction. These are `N` for North, `S` for South, `E` for East, and `W` for West. The value is the number of positions to move.
2) Turn a given amount. These are `L` for Left and `R` for Right. The value is the number of degrees to turn.  
Examining the Input file shows the values to be 90, 180, and 270.
3) Move in forward in the current direction the ship is facing. This action is `F` and the value is the number of positions to move.

At the end, return the Manhattan distance (change in x + change in y) from the origin point.

### Input Format ###

A series of strings each of the format of one character for the Action followed by the value.

### Output Format ###

A number - the Manhattan distance from the starting position.

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day12_test1.txt) given.

### Custom Test Cases ###

None anticipated to be needed.

## Approach ##

### Input Parsing ###
* Going to create a struct with the Action and Value called Instruction.
* Go over each line as a string, splitting the first character to be the Action and converting the remainder as an integer to be the value.
* Return a vector of Instructions

### Directions Array ###
Going to create a `char[4]` array with the values `[E,S,W,N]`.  
By doing this lookup, I can minimize code reuse for the moves.  
Also, I can store the direction as an index into this array and adjust that index when turning left/right.

### Main Loop ###

* Parse the Input as described above
* Set the current_direction to 0 to correspond to `E` in the directions array
* Set the current_x and current_y to 0 for them to be at the origin
* Loop over the Instructions
    * Store the Instruction's Action in current_action
    * If the Instruction's Action is `F` for forward:
        * Overwrite the current_action with the directions[current_direction] value.
    * If the current_action is `N`, `S`, `E`, or `W` (Note this also captures the Forward case)
        * Move the ship by adding or subtracting the Value to current_x or current_y as appropriate
    * If the current_action is 'L' or 'R'
        * Change the current direction by adding (if right) or subtracting (if left) the value/90 to the current_direction **AND ADDING 4** and modding it by 4. This will result in a value between 0 and 3
* Return the Manhattan distance from the origin. Since I started at 0,0, it's just `(abs(current_x) + abs(current_y))`

## Things I learned/found unique ##

Using an array to keep track of the directions so that the modulo arithmetic to adjust it is easier.

### Thing to double-check ###
Can't remember if `-1%4` results in `-1` or `3` in c++. I'll need to do a simple test for this and if needed add 4 to the Left/Right actions before doing the mod to compensate. 
**Update - it does result in -1. Going to have to add 4 on the left/right action to compensate**

