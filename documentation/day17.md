# Day 17, Parts 1 and 2#

## Requirements ##

### Part 1 ###

This program is a 3-D version of Conway's Game of Life. The initial plane of active (`#`) and inactive (`.`) is given. This plane is defined to exist in an infinitely large space of inactive cells.

Since this is 3-D, instead of a cell have the 8 neighbors that 2-D has, it has 26 neighbors.

The following rules apply:
* If a cube is active (`#`) and 2 or 3 of its neighbors are active, that cube remaind active. Else, it flips to inactive (`.`)
* If a cube is inactive (`.`) and 3 of its neighbors are active, that cube flips to active (`#`). Else is stays inactive.

We need to process this for 6 rounds and then return the number of cubes in the active state after 6 rounds.

### Part 2 ###

Same as part 1, but for a 4-D space instead of a 3-D space. Same two rules apply exactly.

### Input Format ###

A 2-D plane is given with active (`#`) and inactive (`.`) cubes.

### Output Format ###

Number - the count of active cubes.

## Test Cases ##

### Given Test Cases ###

There is [one given test case](../data/test_cases/day17_test1.txt)

### Custom Test Cases ###

None planned

## Approach ##

### Considerations ###

Since the space will only grow at most 1 unit in each direciton for each cycle, I could create a large enough 3-D array to contain the largest possible space after 6 rounds. However, in case part 2 requires many more rounds, I'd have to rip that out.

Instead I plan on making the space start with 1 unit of border in each direction. If after the rules are evaluated, if any cell in the border is active, I will grow the space by 1 unit in the dimensions I need to, in order to provide that 1-unit border.

### Data Structures ###

I will create a simple cell struct. It will have 4 values - current_state, current_num_neighbors_active, prev_state, and prev_num_neighbors_active.

*A next_state variable was added during development to hold the next state after the rules have been run but before applying it to the current state. This was needed to detect when to resize*

There will be a space class that will contain a 3-D array of these cell structs. For ease in display, it will be cells[z][y][x].  
With the given 2-D plane of size *m* x *n*, and the need to provide the 1 cell border, the initial size of this will be `x=m+2`, `y=n+2`, `z=3`.
The class will contain the following methods:
* run_cycle - this function will run through a cycle of the game
* resize - this function will resize the space by adding to any/all of the x, y, and z dimensions
* count_active - this function will return the count of active cells. Will be needed for getting the result.
* display - simple function to dump the data to the screen
* set_cell_active - set a cell active, and have the side effect of incrementing all 26 of the neighbors num_neighbors_active counts
* init_from_plane - set up the space based on the input 2-D plane

### Run Cycle ###

Run Cycle will do the following operations:
* Initialize all the cells:
    * copy every cell's current_state to its prev_state and its current_num_neighbors_active to its prev_num_neighbors_active. 
    * Set all current_num_neighbors_active to 0 and current_state to inactive.
* initialize grow_x, grow_y, and grow_z bools to false
* loop over every cell
    * If the cell should be active based on the rules above and the prev_state and prev_num_neighbors_active
        * Set that cells next_state member to Active
        * Check if the cell is on any borders, and set set the appropriate grow_n variable(s) to true
* if any grow_n variable is true
    * call resize, specifying which directions to grow.
* loop over every non-border cell
    * If the cell has it's next_state set to active
        * Call the set_cell_active function on that cell. This will flip the current_state value and adjust the 26 neighbor's current_num_neighbors_active values

### Resize ###

Resize will take 3 parameters: grow_x, grow_y, and grow_z.

* Determine the new size. Each dimension will either stay the same size if the grow_n value is false or increase by 2 if the grow_n value is true.
* Allocate the new member. Initialize each struct to have both states as inactive and both counts as 0.
* Loop over the existing 3-D array with z, y, and x to copy the values such that
    * if grow_n is true, then new_n=old_n+1
    * if grow_n is false, then new_n=old_n.
* Pointer swap the arrays and delete the old array.

### Init From Plane ###

The space class will be created based on the 2-D plane of sized *m* x *n* passed in.
* Create the initial array to be size x=m+2, y=n+2, z=3
    * both states will be set to inactive and both counts will be set to 0
* Loop over the 2-D plane with in_x and in_y
    * If plane[in_x][in_y] is active
        * call set_cell_active with x=in_x+1, y=in_y+1, z=1 . This will map that cell to the sub-space inside the 1-cell border.

### Main Loop ###

* Read in the input into a vector of strings
* Create a space and call init_from_plane on it, passing in the plane.
* Loop 6 times:
    * Call the run_cycle methods
    * Call the display method to dump the space after the cycle
* Get the count of active cells with the count_active method and return that value

### Part 2 ###

Part 2 will use the same Cell structure. I will create a Hyperspace class that is like the Space class, but includes 4 dimensions and the same functionality.

## Things I learned ##

Been trying to design the algorith as best as possible. Fun challenge with that. Deciding the best way to do the resize method.

### Part 2 ###

Hoping for a simple converstion from Part 1 to Part 2, with just one extra dimension
