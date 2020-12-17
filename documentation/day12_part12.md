# Day 12, Part 1 #

## Requirements ##

Given a set of actions and values, move the ship and/or the waypoint around the plane as specified.  

For our sake, the ship starts at the origin. The waypoint starts at (10,1), relative to the ship.

There are three types of actions:
1) Move the waypoint specified direction. These are `N` for North, `S` for South, `E` for East, and `W` for West. The value is the number of positions to move.
2) Rotate the waypoint around the ship by a given amount. These are `L` for Left and `R` for Right. The value is the number of degrees to rotate the waypoint.
Examining the Input file shows the values to be 90, 180, and 270.  
3) Move in forward towards the waypoint. This action is `F` and the value is the number of positions to move.

Based on the input, there are really only three rotate options:
1) **R90**/**L270** = a right rotation by 90 degrees
2) **R270**/**L90** = a left rotation by 90 degrees
3) **R180**/**L180** = a rotation by 180 degrees (could be left or right)

At the end, return the Manhattan distance (change in x + change in y) from the origin point.

### Input Format ###

A series of strings each of the format of one character for the Action followed by the value.

### Output Format ###

A number - the Manhattan distance from the starting position.

## Test Cases ##

### Given Test Cases ###

There is [one test case](../data/test_cases/day12_test1.txt) given.

### Custom Test Cases ###

I created a [test case](../data/test_cases/day12_test2.txt) to work through the math on the rotations.

After my first solution was wrong, I took the working test case in test 1, and copied it to start it [90](../data/test_cases/day12_test3.txt), [180](../data/test_cases/day12_test4.txt), and [270](../data/test_cases/day12_test5.txt) degrees to the left to help isolate the failures.

## Approach ##

### Input Parsing ###
* Going to create a struct with the Action and Value called Instruction.
* Go over each line as a string, splitting the first character to be the Action and converting the remainder as an integer to be the value.
* Return a vector of Instructions

### Waypoint Rotation ###

For all 3 rotations, store the current waypoint_x and waypoint_y in prev_x and prev_y, since we have to overwrite them both.

#### Right 180 / Left 180 ####
* Set waypoint_x to -1 * prev_x 
* Set waypoint_y to -1 * prev_y 

#### Right 90 / Left 270 ###
* If Quadrant 1 (prev_x >= 0, prev_y >= 0) *(10,1)* --> *(1,-10)*
    * Moving to Quandrant 4 (x > 0, y < 0)
    * Set waypoint_x to prev_y
    * Set waypoint_y to (-1 * prev_x)
* Else if Quadrant 2 (prev_x < 0, prev_y >= 0) *(-1,10)* --> *(10,1)*
    * Moving to Quadrant 1 (x > 0, y > 0)
    * Set waypoint_x to prev_y
    * Set waypoint_y to (-1 * prev_x)
* Else if Quadrant 3 (prev_x < 0, prev_y < 0) *(-10,-1)* --> *(-1,10)*
    * Moving to Quadrant 2 (x < 0, y > 0)
    * Set waypoint_x to prev_y
    * set waypoint_y to (-1 * prev_x)
* Else if Quadrant 4 (prev_x >= 0, prev_y < 0) *(1,-10)* --> *(-10, -1)*
    * Moving to Quadrant 3 (x < 0, y < 0)
    * Set waypoint_x to prev_y
    * set waypoint_y to (-1 * prev_x)

After listing these out, I see that all the cases can be dropped, and just do:
* Set waypoint_x to prev_y
* set waypoint_y to (-1 * prev_x)

#### Left 90 / Right 270 ###
* If Quadrant 1 (prev_x >= 0, prev_y >= 0) *(10,1)* --> *(-1,10)*
    * Moving to Quandrant 2 (x < 0, y > 0)
    * Set waypoint_x to (-1 * prev_y)
    * Set waypoint_y to prev_x
* Else if Quadrant 2 (prev_x < 0, prev_y >= 0) *(-1,10)* --> *(-10,-1)*
    * Moving to Quadrant 3 (x < 0, y < 0)
    * Set waypoint_x to (-1 * prev_y)
    * Set waypoint_y to prev_x
* Else if Quadrant 3 (prev_x < 0, prev_y < 0) *(-10,-1)* --> *(1,-10)*
    * Moving to Quadrant 4 (x > 0, y < 0)
    * Set waypoint_x to (-1 * prev_y)
    * Set waypoint_y to prev_x
* Else if Quadrant 4 (prev_x >= 0, prev_y < 0) *(1,-10)* --> *(10, 1)*
    * Moving to Quadrant 1 (x > 0, y > 0)
    * Set waypoint_x to (-1 * prev_y)
    * Set waypoint_y to prev_x

After listing these out, I see that all the cases can be dropped, and just do:
* Set waypoint_x to (-1 * prev_y)
* Set waypoint_y to prev_x

Well, that simplified things, didn't it :-)

### Main Loop ###

* Parse the Input as described above
* Set the current_x and current_y to 0 for them to be at the origin
* Set the waypoint_x to 10 and waypoint_y to be 1 to correspond for them being at 10,1
* Loop over the Instructions
    * If the Instruction's Action is `F` for forward:
        * Move the ship to the waypoint *value* times. This is done by adding the `waypoint_x*value` to `current_x` and `waypoint_y*value` to `current_y`
    * If the Instruction's Action is `N`, `S`, `E`, or `W`
        * Move the waypoint by adding or subtracting the Value to waypoint_x or waypoint_y as appropriate
    * If the Instruction's Actionis 'L' or 'R'
        * Change the waypoint a described in the Waypoint Rotation section.
* Return the Manhattan distance from the origin. Since I started at 0,0, it's just `(abs(current_x) + abs(current_y))`

## Things I learned/found unique ##

Working through the math on the waypoint rotations; as well as simplifying the use cases for it was helpful. Creating a separate test case for it was also useful.

After I got the wrong solution and didn't see anything obvious, I thought it was unique to take the sample from the problem and try it with different starting rotations. This helped isolate where the problem was.
