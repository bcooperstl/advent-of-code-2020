#include <iostream>
#include <vector>
#include <map>

#include "aoc_day_20.h"

using namespace std;

vector<string> get_expected_map(Manipulation manipulation)
{
    vector<string> ret;
    switch (manipulation)
    {
        case RightRotate0Degrees:
            ret.push_back("........#.");
            ret.push_back(".###......");
            ret.push_back(".###......");
            ret.push_back(".###......");
            ret.push_back("#...##....");
            ret.push_back("....##....");
            ret.push_back("..........");
            ret.push_back(".##......#");
            ret.push_back(".##.......");
            ret.push_back("...#......");
            break;
    }
    return ret;
}

Tile generate_base_tile()
{
    vector<string> test_data;
    test_data.push_back("........#.");
    test_data.push_back(".###......");
    test_data.push_back(".###......");
    test_data.push_back(".###......");
    test_data.push_back("#...##....");
    test_data.push_back("....##....");
    test_data.push_back("..........");
    test_data.push_back(".##......#");
    test_data.push_back(".##.......");
    test_data.push_back("...#......");
    Tile tile(1111);
    
    tile.set_map(test_data);
    return tile;
}
 
void compare_borders(map<Border, int> expected, map<Border, int> actual)
{
    if (expected[NorthFront] != actual[NorthFront])
    {
        cout << "***** NorthFront border mismatch!! Expected " << expected[NorthFront] << " Received " << actual[NorthFront] << "*****" << endl;
    }
    else
    {
        cout << "NorthFront border matches" << endl;
    }
    if (expected[EastFront] != actual[EastFront])
    {
        cout << "***** EastFront border mismatch!! Expected " << expected[EastFront] << " Received " << actual[EastFront] << "*****" << endl;
    }
    else
    {
        cout << "EastFront border matches" << endl;
    }
    if (expected[SouthFront] != actual[SouthFront])
    {
        cout << "***** SouthFront border mismatch!! Expected " << expected[SouthFront] << " Received " << actual[SouthFront] << "*****" << endl;
    }
    else
    {
        cout << "SouthFront border matches" << endl;
    }
    if (expected[WestFront] != actual[WestFront])
    {
        cout << "***** WestFront border mismatch!! Expected " << expected[WestFront] << " Received " << actual[WestFront] << "*****" << endl;
    }
    else
    {
        cout << "WestFront border matches" << endl;
    }
    if (expected[NorthBack] != actual[NorthBack])
    {
        cout << "***** NorthBack border mismatch!! Expected " << expected[NorthBack] << " Received " << actual[NorthBack] << "*****" << endl;
    }
    else
    {
        cout << "NorthBack border matches" << endl;
    }
    if (expected[EastBack] != actual[EastBack])
    {
        cout << "***** EastBack border mismatch!! Expected " << expected[EastBack] << " Received " << actual[EastBack] << "*****" << endl;
    }
    else
    {
        cout << "EastBack border matches" << endl;
    }
    if (expected[SouthBack] != actual[SouthBack])
    {
        cout << "***** SouthBack border mismatch!! Expected " << expected[SouthBack] << " Received " << actual[SouthBack] << "*****" << endl;
    }
    else
    {
        cout << "SouthBack border matches" << endl;
    }
    if (expected[WestBack] != actual[WestBack])
    {
        cout << "***** WestBack border mismatch!! Expected " << expected[WestBack] << " Received " << actual[WestBack] << "*****" << endl;
    }
    else
    {
        cout << "WestBack border matches" << endl;
    }    
}

void compare_map(vector<string> expected, vector<string> actual)
{
    bool equal = true;
    for (int i=0; i<TILE_SIDE_LEN; i++)
    {
        if (expected[i] != actual[i])
        {
            equal = false;
        }
    }
    if (equal)
    {
        cout << "Maps match" << endl;
    }
    else
    {
        cout << "Map mismatch!!!" << endl;
        cout << "Expected    Actual" << endl;
        for (int i=0; i<TILE_SIDE_LEN; i++)
        {
            cout << expected[i] << "  " << actual[i] << endl;
        }
    }
}

void test_base_tile()
{
    map<Border, int> expected_borders;
    expected_borders[NorthFront] =2;
    expected_borders[NorthBack]  =256;
    expected_borders[EastFront]  =4;
    expected_borders[EastBack]   =128;
    expected_borders[SouthFront] =8;
    expected_borders[SouthBack]  =64;
    expected_borders[WestFront]  =16;
    expected_borders[WestBack]   =32;
    
    Tile tile = generate_base_tile();
    map<Border, int> actual_borders = tile.get_borders();
    cout << "Testing after construction of tile" << endl;
    compare_borders(expected_borders, actual_borders);
    compare_map(get_expected_map(RightRotate0Degrees), tile.get_tile_map());
}

int main (int argc, char * argv[])
{
    test_base_tile();

    return 0;
}
