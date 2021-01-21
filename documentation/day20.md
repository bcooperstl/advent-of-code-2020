# Day 20 #

## Requirements ##

### General Requirements ###

We are given a square image that is broken down into square tiles. Each tile has an ID value.
Each tile is 10x10 characters, which includes a 1-character border on each edge that will be used to match up the tiles with each other.
Tiles on the border of the image have this 1-character border, but it will not match up with any other tiles.

Tiles can be rotated and flipped to make the borders line up.

The tiles need to be arranged into the main image.

### Part 1 Requirements ###

Identify the four corner pieces of the image, and return the product of their IDs.

The problem specifically states *Assemble the tiles into an image.* I realize I could solve this just by identifying the corner tiles, but I'll go ahead and try to assemble it.

### Input Format ###

There is one line of the format `Tile nnnn:` where the *nnnn* is the ID number. By inspection, these all appear to be 4-digit numbers.

There are the 10 lines which show the tile. The two characters used for the tile are `.` and `#`.

There is a blank line between tiles.

### Output Format ###

#### Part 1 ####

Number - the product of the IDs of the four corner tiles.

## Test Cases ##

### Given Test Cases ###

There is [one given test case](../data/test_cases/day20_test1.txt) for Part 1.

### Custom Test Cases ###

I hope I don't need to make another test case.


## Approach ##

This will be done in several iterations, and descried as such.

### Representing a tile's border as an integer ###

Instead of having to do many many string comparisons, a tile's border can be represented as an integer.  
I will encode `.` characters a 0 bits, and `#` characters as 1 bits.
I will first encode all four sides. Since tiles can be flipped, there will be two encodings per side - front and back.  
The fronts will be encoded with the characters in clockwise order. The backs will be encoded in counter-clockwise order.

#### An Example ####
    Tile 2311:
    ..##.#..#.
    ##..#.....
    #...##..#.
    ####.#...#
    ##.##.###.
    ##...#.###
    .#.#.#..##
    ..#....#..
    ###...#.#.
    ..###..###

Reading left-to-right, the North border is `..##.#..#.`. 
Since this is clockwise for the front, this encodes as `0011010010=210`. 
Working counter-clockwize for the back, this encodes as `0100101100=300`.

Reading top-to-bottom, the East border is `...#.##..#`.
Since this is clockwise for the front, this encodes as `0001011001=89`. 
Working counter-clockwize for the back, this encodes as `1001101000=616`.

Reading left-to-right, the South border is `..###..###`.
However, this needs to be clocksize for the front, so it encodes as `1110011100=924`.
Working counter-clocksize, with the original text, for the back, this encoded as `0011100111=231`.

Reading top-to-bottom, the West border is `.#####..#.`.
However, this needs to be clocksize for the front, so it encodes as `0100111110=318`.
Working counter-clocksize, with the original text, for the back, this encoded as `0111110010=498`.

### Enumerations ####

#### Border ####

The border enumeration will identify the 8 possible borders.  
* 0=NorthFront
* 1=EastFront
* 2=SouthFront
* 3=WestFront
* 4=NorthBack
* 5=EastBack
* 6=SouthBack
* 7=WestBack

#### Manipulations ####

Going back to my Abstract Algebra class, there are 8 manipulations on a square that can be done. There are four rotations, and four ways to flip.
* 0=RightRotate0Degrees - this will in essence do nothing. Front values stay on front.
* 1=RightRotate90Degrees - this will rotate 90 degrees to the right. Front values stay on front.
* 2=RightRotate180Degrees - this will rorate 180 degress to the right. Front values stay on front.
* 3=RightRotate270Degrees -this will rotate 270 degrees to the right. Front values stay on front.
* 4=HorizontalFlip - this will flip it over the x-axis. North moves to South, South moves to North. East and West stay on their sides. Back values are now on front.
* 5=VerticalFlip - this will flip it over the y-axis. North and South stay on their sides. East moves to West and West moves to East. Back values are now on front.
* 6=ForwardSlashFlip `/` - this will flip is over an axis equivalent to y=x. North moves to East, East moves to North. South moves to West and West moves to South. Back values are now on front.
* 7=BackwardSlashFlip `\` - this will flip it over an axis equivalent to y=-x. North moves to West, East moves to South, South moves to East, and West moves to North. Back values are now on front.

### Tile Class ###

The tile class will do a lot of the work. It will contain the following:
* ID value
* 2-d Character array mapping the tile
* Array of 8 border values


### Parsing the Input ###

To parse the input, I will first read it in as a list of strings.

A tile is fully defined in 11 lines, with a blank line in between.  
Working over each tile, I will:
* Extract the tile_id from the first line, positions 5-8
* Populate the tile map from lines 2-11.

### Building Border Values ###


## Test Programs ##

I will need a test program to verify the manipulations.
After doing each of the manipulations on this sample tile, the program will both check the tile's values for the 8 borders, as well as re-computer them from the characters in the tile.

    Tile 1111:
    ........#.
    .###......
    .###......
    .###......
    #...##....
    ....##....
    ..........
    .##......#
    .##.......
    ...#......

These are the following initial values for the borders:

Initially: NorthFront=2, NorthBack=256, EastFront=4, EastBack=128, SouthFront=8, SouthBack=64, WestFront=16, WestBack=32.


#### Rotate 0 Results ####
NorthFront=2, NorthBack=256, EastFront=4, EastBack=128, SouthFront=8, SouthBack=64, WestFront=16, WestBack=32.
Image looks the same.

#### Rotate 90 Right Results ####
NorthFront=16, NorthBack=32, EastFront=2, EastBack=256, SouthFront=4, SouthBack=128, WestFront=8, WestBack=64.
Image rotates so that the 3x3 square is in the top-right corner, the 2x2 square is in the top-left corner.

#### Rotate 180 Right Results ####
NorthFront=8, NorthBack=64, EastFront=16, EastBack=32, SouthFront=2, SouthBack=256, WestFront=4, WestBack=128.
Image rotates so that the 3x3 square is in the bottom-right corner, the 2x2 square is in the top-right corner.

#### Rotate 270 Right Results ####
NorthFront=4, NorthBack=128, EastFront=8, EastBack=64, SouthFront=16, SouthBack=32, WestFront=2, WestBack=256.
Image rotates so that the 3x3 square is in the bottom-left corner, the 2x2 square is in the bottom-right corner.

#### Flip Horizontal Results ####
NorthFront=64, NorthBack=8, EastFront=128, EastBack=4, SouthFront=256, SouthBack=2, WestFront=32, WestBack=16.
Image flipped so that 3x3 square is in the bottom-left corner and 2x2 square is in the top-left corner.

#### Flip Vertial Results ####
NorthFront=256, NorthBack=2, EastFront=32, EastBack=16, SouthFront=64, SouthBack=8, WestFront=128, WestBack=4.
Image flipped so that 3x3 square is in the top-right corner and 2x2 square is in the bottom-right corner.

#### Flip Forward Slash Results ####
Flipping over the `y=x` `/` axis.  
NorthFront=128, NorthBack=4, EastFront=256, EastBack=2, SouthFront=32, SouthBack=16, WestFront=64, WestBack=8
Image flipped so the 3x3 square is in the bottom-right corner, and the 2x2 square stays in the bottom-left corner.

#### Flip Backward Slash Results ####
Flipping over the `y=-x` `\` axis.  
NorthFront=32, NorthBack=16, EastFront=64, EastBack=8, SouthFront=128, SouthBack=4, WestFront=256, WestBack=2
Image flipped so the 3x3 square stays in the top-left corner and the 2x2 squares moves to the top-right.



## Things I learned ##


