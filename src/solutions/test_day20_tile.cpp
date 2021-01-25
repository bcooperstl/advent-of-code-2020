#include <iostream>
#include <vector>
#include <map>

#include "aoc_day_20.h"

using namespace std;

map<Border, int> get_expected_borders(Manipulation manipulation)
{
    map<Border, int> expected_borders;
    switch (manipulation)
    {
        case RightRotate0Degrees:
            expected_borders[NorthFront] = 2;
            expected_borders[NorthBack]  = 256;
            expected_borders[EastFront]  = 4;
            expected_borders[EastBack]   = 128;
            expected_borders[SouthFront] = 8;
            expected_borders[SouthBack]  = 64;
            expected_borders[WestFront]  = 16;
            expected_borders[WestBack]   = 32;
            break;
        case RightRotate90Degrees:
            expected_borders[NorthFront] = 16;
            expected_borders[NorthBack]  = 32;
            expected_borders[EastFront]  = 2;
            expected_borders[EastBack]   = 256;
            expected_borders[SouthFront] = 4;
            expected_borders[SouthBack]  = 128;
            expected_borders[WestFront]  = 8;
            expected_borders[WestBack]   = 64;
            break;
        case RightRotate180Degrees:
            expected_borders[NorthFront] = 8;
            expected_borders[NorthBack]  = 64;
            expected_borders[EastFront]  = 16;
            expected_borders[EastBack]   = 32;
            expected_borders[SouthFront] = 2;
            expected_borders[SouthBack]  = 256;
            expected_borders[WestFront]  = 4;
            expected_borders[WestBack]   = 128;
            break;
        case RightRotate270Degrees:
            expected_borders[NorthFront] = 4;
            expected_borders[NorthBack]  = 128;
            expected_borders[EastFront]  = 8;
            expected_borders[EastBack]   = 64;
            expected_borders[SouthFront] = 16;
            expected_borders[SouthBack]  = 32;
            expected_borders[WestFront]  = 2;
            expected_borders[WestBack]   = 256;
            break;
        case HorizontalFlip:
            expected_borders[NorthFront] = 64;
            expected_borders[NorthBack]  = 8;
            expected_borders[EastFront]  = 128;
            expected_borders[EastBack]   = 4;
            expected_borders[SouthFront] = 256;
            expected_borders[SouthBack]  = 2;
            expected_borders[WestFront]  = 32;
            expected_borders[WestBack]   = 16;
            break;
        case VerticalFlip:
            expected_borders[NorthFront] = 256;
            expected_borders[NorthBack]  = 2;
            expected_borders[EastFront]  = 32;
            expected_borders[EastBack]   = 16;
            expected_borders[SouthFront] = 64;
            expected_borders[SouthBack]  = 8;
            expected_borders[WestFront]  = 128;
            expected_borders[WestBack]   = 4;
            break;
        case ForwardSlashFlip:
            expected_borders[NorthFront] = 128;
            expected_borders[NorthBack]  = 4;
            expected_borders[EastFront]  = 256;
            expected_borders[EastBack]   = 2;
            expected_borders[SouthFront] = 32;
            expected_borders[SouthBack]  = 16;
            expected_borders[WestFront]  = 64;
            expected_borders[WestBack]   = 8;
            break;
        case BackwardSlashFlip:
            expected_borders[NorthFront] = 32;
            expected_borders[NorthBack]  = 16;
            expected_borders[EastFront]  = 64;
            expected_borders[EastBack]   = 8;
            expected_borders[SouthFront] = 128;
            expected_borders[SouthBack]  = 4;
            expected_borders[WestFront]  = 256;
            expected_borders[WestBack]   = 2;
            break;
    }
    return expected_borders;
}

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
        case RightRotate90Degrees:
            ret.push_back(".....#....");
            ret.push_back(".##...###.");
            ret.push_back(".##...###.");
            ret.push_back("#.....###.");
            ret.push_back("....##....");
            ret.push_back("....##....");
            ret.push_back("..........");
            ret.push_back("..........");
            ret.push_back(".........#");
            ret.push_back("..#.......");
            break;
        case RightRotate180Degrees:
            ret.push_back("......#...");
            ret.push_back(".......##.");
            ret.push_back("#......##.");
            ret.push_back("..........");
            ret.push_back("....##....");
            ret.push_back("....##...#");
            ret.push_back("......###.");
            ret.push_back("......###.");
            ret.push_back("......###.");
            ret.push_back(".#........");
            break;
        case RightRotate270Degrees:
            ret.push_back(".......#..");
            ret.push_back("#.........");
            ret.push_back("..........");
            ret.push_back("..........");
            ret.push_back("....##....");
            ret.push_back("....##....");
            ret.push_back(".###.....#");
            ret.push_back(".###...##.");
            ret.push_back(".###...##.");
            ret.push_back("....#.....");
            break;
        case HorizontalFlip:
            ret.push_back("...#......");
            ret.push_back(".##.......");
            ret.push_back(".##......#");
            ret.push_back("..........");
            ret.push_back("....##....");
            ret.push_back("#...##....");
            ret.push_back(".###......");
            ret.push_back(".###......");
            ret.push_back(".###......");
            ret.push_back("........#.");
            break;
        case VerticalFlip:
            ret.push_back(".#........");
            ret.push_back("......###.");
            ret.push_back("......###.");
            ret.push_back("......###.");
            ret.push_back("....##...#");
            ret.push_back("....##....");
            ret.push_back("..........");
            ret.push_back("#......##.");
            ret.push_back(".......##.");
            ret.push_back("......#...");
            break;
        case ForwardSlashFlip:
            ret.push_back("..#.......");
            ret.push_back(".........#");
            ret.push_back("..........");
            ret.push_back("..........");
            ret.push_back("....##....");
            ret.push_back("....##....");
            ret.push_back("#.....###.");
            ret.push_back(".##...###.");
            ret.push_back(".##...###.");
            ret.push_back(".....#....");
            break;
        case BackwardSlashFlip:
            ret.push_back("....#.....");
            ret.push_back(".###...##.");
            ret.push_back(".###...##.");
            ret.push_back(".###.....#");
            ret.push_back("....##....");
            ret.push_back("....##....");
            ret.push_back("..........");
            ret.push_back("..........");
            ret.push_back("#.........");
            ret.push_back(".......#..");
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
    Tile tile = generate_base_tile();
    map<Border, int> actual_borders = tile.get_borders();
    cout << "Testing after construction of tile" << endl;
    compare_borders(get_expected_borders(RightRotate0Degrees), actual_borders);
    compare_map(get_expected_map(RightRotate0Degrees), tile.get_tile_map());
}

void test_manipulation(Manipulation manipulation, string label_for_output)
{
    Tile tile = generate_base_tile();
    tile.perform_manipulation(manipulation);
    map<Border, int> actual_borders = tile.get_borders();
    cout << "Testing after performing manipulation of " << label_for_output << ":" << endl;
    compare_borders(get_expected_borders(manipulation), actual_borders);
    compare_map(get_expected_map(manipulation), tile.get_tile_map());
}

int main (int argc, char * argv[])
{
    test_base_tile();
    
    test_manipulation(RightRotate0Degrees, "Right rotate 0 degrees");
    test_manipulation(RightRotate90Degrees, "Right rotate 90 degrees");
    test_manipulation(RightRotate180Degrees, "Right rotate 180 degrees");
    test_manipulation(RightRotate270Degrees, "Right rotate 270 degrees");
    test_manipulation(HorizontalFlip, "Horizontal flip");
    test_manipulation(VerticalFlip, "Vertical flip");
    test_manipulation(ForwardSlashFlip, "Forward slash / flip");
    test_manipulation(BackwardSlashFlip, "Backward slash \\ flip");

    return 0;
}
