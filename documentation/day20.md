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
1) The first iteration involves getting the Tile class working.
2) Classifying the tiles are corners, edges, or middle pieces.
3) Answering part 1. Since I have the four corners, I can just multiply their IDs to get the answer. This still doesn't build the puzzle.

### Representing a tile's border as an integer ###

Instead of having to do many many string comparisons, a tile's border can be represented as an integer.  
I will encode `.` characters a 0 bits, and `#` characters as 1 bits.
I will first encode all four sides. Since tiles can be flipped, there will be two encodings per side - front and back.  
The fronts will be encoded with the characters in clockwise order. The backs will be encoded in counter-clockwise order.

In phase 2, I added a check to see if any sides are palindromes. This would make more complexity for the solution, so I ran that check first. There were none, so that is good.


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
Working counter-clockwise for the back, this encodes as `0100101100=300`.

Reading top-to-bottom, the East border is `...#.##..#`.
Since this is clockwise for the front, this encodes as `0001011001=89`. 
Working counter-clockwise for the back, this encodes as `1001101000=616`.

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

* Initialize all 8 border values to 0.
* For i from 0 to 9
    * if map[0][i] is '#' (The i'th value on the north edge is #; bit=1)
        * Set border[NorthFront] |= (1 << 9-i) // For clockwise NorthFront, most sig bit is first (i=0) and least sig bit is last (i=9)
        * Set border[NorthBack] |= (1 << i) // For counter-clockwise NorthBack, most sig bit is last (i=9) and least sig bit is first (i=0)
    * if map[9][i] is '#' (The i'th value on the south edge is #; bit=1)
        * Set border[SouthFront] |= (1 << i) // For clockwise SouthFront, most sig bit is last (i=9) and least sig bit is first (i=0)
        * Set border[SouthBack] |= (1 << 9-i) // For counter-clockwise SouthBack, most sig bit is first (i=0) and least sig bit is last (i=9)
    * if map[i][0] is '#' (The i'th value on the west edge is #; bit=1)
        * Set border[WestFront] |= (1 << i) // For clockwise WestFront, most sig bit is last (i=9) and least sig bit is first (i=0)
        * Set border[WestBack] |= (1 << 9-i) // For counter-clockwise WestBack, most sig bit is first (i=0) and least sig bit is last (i=9)
    * if map[i][9] is '#' (The i'th value on the east edge is #; bit=1)
        * Set border[EastFront] |= (1 << 9-i) // For clockwise EastFront, most sig bit is first (i=0) and least sig bit is last (i=9)
        * Set border[EastBack] |= (1 << i) // For counter-clockwise EastBack, most sig bit is last (i=9) and least sig bit is first (i=0)

### Manipulations ###

All of these (except right rotate 0 degrees) will copy the current border values to old_borders and their current map to old_map. 
They will then update the internal structures as described below.

#### Right Rotate 0 Degrees ####
* Do nothing. If only they all were so easy.

#### Right Rotate 90 Degrees ####
* borders[EastFront]  = old[NorthFront]
* borders[EastBack]   = old[NorthBack]
* borders[SouthFront] = old[EastFront]
* borders[SouthBack]  = old[EastBack]
* borders[WestFront]  = old[SouthFront]
* borders[WestBack]   = old[SouthBack]
* borders[NorthFront] = old[WestFront]
* borders[NorthBack]  = old[WestBack]
* for old_x from 0 to 9
    * for old_y from 0 to 9
        * map[old_x][9-old_y]=old_map[old_y][old_x]

#### Right Rotate 180 Degrees ####
* borders[SouthFront] = old[NorthFront]
* borders[SouthBack]  = old[NorthBack]
* borders[WestFront]  = old[EastFront]
* borders[WestBack]   = old[EastBack]
* borders[NorthFront] = old[SouthFront]
* borders[NorthBack]  = old[SouthBack]
* borders[EastFront]  = old[WestFront]
* borders[EastBack]   = old[WestBack]
* for old_x from 0 to 9
    * for old_y from 0 to 9
        * map[9-old_y][9-old_x]=old_map[old_y][old_x]

#### Right Rotate 270 Degrees ####
* borders[WestFront]  = old[NorthFront]
* borders[WestBack]   = old[NorthBack]
* borders[NorthFront] = old[EastFront]
* borders[NorthBack]  = old[EastBack]
* borders[EastFront]  = old[SouthFront]
* borders[EastBack]   = old[SouthBack]
* borders[SouthFront] = old[WestFront]
* borders[SouthBack]  = old[WestBack]
* for old_x from 0 to 9
    * for old_y from 0 to 9
        * map[9-old_x][old_y]=old_map[old_y][old_x]

#### Horizonal Flip ####
* borders[SouthBack]  = old[NorthFront]
* borders[SouthFront] = old[NorthBack]
* borders[EastBack]   = old[EastFront]
* borders[EastFront]  = old[EastBack]
* borders[NorthBack]  = old[SouthFront]
* borders[NorthFront] = old[SouthBack]
* borders[WestBack]   = old[WestFront]
* borders[WestFront]  = old[WestBack]
* for old_x from 0 to 9
    * for old_y from 0 to 9
        * map[9-old_y][old_x]=old_map[old_y][old_x]

#### Vertical Flip ####
* borders[NorthBack]  = old[NorthFront]
* borders[NorthFront] = old[NorthBack]
* borders[WestBack]   = old[EastFront]
* borders[WestFront]  = old[EastBack]
* borders[SouthBack]  = old[SouthFront]
* borders[SouthFront] = old[SouthBack]
* borders[EastBack]   = old[WestFront]
* borders[EastFront]  = old[WestBack]
* for old_x from 0 to 9
    * for old_y from 0 to 9
        * map[old_y][9-old_x]=old_map[old_y][old_x]

#### Forward Slash Flip / ####
* borders[EastBack]   = old[NorthFront]
* borders[EastFront]  = old[NorthBack]
* borders[NorthBack]  = old[EastFront]
* borders[NorthFront] = old[EastBack]
* borders[WestBack]   = old[SouthFront]
* borders[WestFront]  = old[SouthBack]
* borders[SouthBack]  = old[WestFront]
* borders[SouthFront] = old[WestBack]
* for old_x from 0 to 9
    * for old_y from 0 to 9
        * map[9-old_x][9-old_y]=old_map[old_y][old_x]

#### Backward Slash Flip \ ####
* borders[WestBack]   = old[NorthFront]
* borders[WestFront]  = old[NorthBack]
* borders[SouthBack]  = old[EastFront]
* borders[SouthFront] = old[EastBack]
* borders[EastBack]   = old[SouthFront]
* borders[EastFront]  = old[SouthBack]
* borders[NorthBack]  = old[WestFront]
* borders[NorthFront] = old[WestBack]
* for old_x from 0 to 9
    * for old_y from 0 to 9
        * map[old_x][old_y]=old_map[old_y][old_x]

### Classifying Tiles ###

Tiles will be classified as either corners, edges, or middle pieces, based on whether there are 2, 1, or 0 sides that only occur once.

First I will build up a `map<int, vector<Tile *>>` border_index that will point each border value to a list of tiles with that value.  
This will use the minimum value for a Front/Back border pair, to eliminate duplicating sides and to make identificaiton easier.  

Then, I will go through each tile, count how many sides only have one element in the map for their side's border value, and assign those tiles to the corner, edge, and middle vectors.

## Part 1 Main Program ##
In order to get to a solution for part 1:

* Parse the input as described above
* Build a border lookup map
* Classify the tiles into those that are corners, edges, and middles.
* Multiply the IDs of the four corner tiles together. This will give the result.

## Test Programs ##

See [day20_test_program.md](Day 20 Test Program) for a description of the test program written to verify the flips and rotations.

## Things I learned ##

So much fun developing the algorithm and flushing it out. Going back to my very basic group-theory class to think about the possible arrangements of a square.

Using static_cast to convert from ints to enums.


