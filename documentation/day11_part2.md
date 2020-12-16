# Day 11, Part 2 #

## Requirements ##

You are given a map of a seat layout, as described in the input format.  

In a Conway's-game-of-life style simulation, you need to work through the following logic to determine in what happens in each iteration:
* If a seat is empty (`L`) and there are no occupied seats in the line of sight, it becomes occupied (`#`)
* If a seat is occupied (`#`) and there are **five** or more seats in the line of sight to it that are also occupied, the seat becomes empty (`L`)
* The floor (`.`) does not change.

Note that for this problem, you are looking in all 8 directions to find the first seat in that direction. This includes diaganols.

This is an enhanced version of [Day 11 Part 1](day11_part1.md). The key changes are looking beyond the adjacent seats on the diagonals and it now requiring 5 occupied seats to become empty.

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

* Add a count_line_of_sight_occupied_seats function. This will walk down the lines until an empty/occupied seat is found, skipping over the floor squares

This problem will use my pre-existing "Screen" objects to store the data for the grid.

* Read in the input file into a vector of strings using my existing function.,
* Create an empty Screen object, and store this in *prior*
* Create a Screen object from the input, and store this in *current*
* While *current* is not equal to *prior*
    * Copy *current* to *prior*
    * Loop over every column and row in *current* with *x* and *y*
        * If `prior[x][y]` is an empty seat (`L`)
            * If there are no line-of-sight occupied seats
                * Set current[x][y] to be occupied (`#`)
        * If `prior[x][y]` is an occupied seat (`#`)
            * If there are five or more line-of-site occupied seats
                * Set current[x][y] to be empty (`L`)
* Count the number of occupied seats in the steady-state map and return that value.

### Speedups ###
My first pass through was too slow. The simple modification of passing my Screen structure by reference instead of by value sped things up **substantially**.  
This makes sense - passing by value involves copying the class - i.e. duplicating the grid. Since I wasn't modifying it, pass-by-reference is the way to go.

## Things I learned ##

See Speedups
