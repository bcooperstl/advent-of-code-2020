# Day 11, Part 1 #

## Requirements ##

You are given a map of a seat layout, as described in the input format.  

In a Conway's-game-of-life style simulation, you need to work through the following logic to determine in what happens in each iteration:
* If a seat is empty (`L`) and there are no occupied seats next to it, it becomes occupied (`#`)
* If a seat is occupied (`#`) and there are four or more seats adjacent to it that are also occupied, the seat becomes empty (`L`)
* The floor (`.`) does not change.

Note that for this problem, you are looking at the up to 8 neighboring squares, which included diagonals.

### Input Format ###

A rectuangular grid showing the seat layout. The key for the grid is:
* `.` : floor
* `L` : an empty seat
* `#` : an occupied seat

### Output Format ###

Integer - the number of occupied seats when a steady state is found.

## Test Cases ##

### Given Test Cases ###

There is [one given test case](../data/test_cases/day11_part2.md). I belive this will be enough

### Custom Test Cases ###

None anticipated to be needed.

## Approach ##

This problem will use my pre-existing "Screen" objects to store the data for the grid.

* Read in the input file into a vector of strings using my existing function.,
* Create an empty Screen object, and store this in *prior*
* Create a Screen object from the input, and store this in *current*
* While *current* is not equal to *prior*
    * Copy *current* to *prior*
    * Loop over every column and row in *current* with *x* and *y*
        * If `prior[x][y]` is an empty seat (`L`)
            * If there are no adjacent occupied seats
                * Set current[x][y] to be occupied (`#`)
        * If `prior[x][y]` is an occupied seat (`#`)
            * If there are four or more adjacent occupied seats
                * Set current[x][y] to be empty (`L`)
* Count the number of occupied seats in the steady-state map and return that value.

### Potential speedups ###
    * If this is running slow, instead of counting the number of neighbors that are occupied, I can make a 2-d array to store this in.

## Things I learned ##

More nuances with copy-constructors, virtual destructors, and overloading the assignment operator for classes and inherited classes.

