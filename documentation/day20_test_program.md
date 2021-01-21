## Day 20 Test Program ##

I will need a test program to verify the manipulations (rotations and flips).
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

