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

See [day20_test_program.md](Day 20 Test Program) for a description of the test program written to verify the flips and rotations.

## Things I learned ##


