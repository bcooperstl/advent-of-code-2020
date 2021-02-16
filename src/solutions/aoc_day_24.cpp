#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

#include "aoc_day_24.h"
#include "file_utils.h"
#include "screen.h"

using namespace std;

#define VOID ' '
#define WHITE_TILE '.'
#define BLACK_TILE '@'
#define PART1_BUFFER 5

AocDay24::AocDay24():AocDay(24)
{
}

AocDay24::~AocDay24()
{
}

vector<string> AocDay24::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return raw_lines;
    }
    return raw_lines;
}

/* grid layout:
 a b 
c o d
 e f 

x values increase left to right
y values increase top to bottom

east goes from o to d: x+2, y
southeast goes from o to f: x+1, y+1
southwest goes from o to e: x-1, y+1
west goes from o to c: x-2, y
northeast goes from o to a: x-1, y-1
northwest goes from o to b: x+1, y-1
*/

vector<Day24Tile> AocDay24::make_tiles(vector<string> inputs)
{
    vector<Day24Tile> tiles;
    for (int i=0; i<inputs.size(); i++)
    {
        Day24Tile tile;
        tile.input = inputs[i];
        tile.x=0;
        tile.y=0;
        
        for (int j=0; j<tile.input.size(); j++)
        {
            char ch = tile.input[j];
            if (ch == CHAR_EAST) // move right 2 spaces
            {
                tile.x+=2;
            }
            else if (ch == CHAR_WEST) // move left 2 spaces
            {
                tile.x-=2;
            }
            else if (ch == CHAR_SOUTH)
            {
                j++;
                ch = tile.input[j];
                if (ch == CHAR_EAST) // southeast - right 1 and down 1
                {
                    tile.x+=1;
                    tile.y+=1;
                }
                else if (ch == CHAR_WEST) // southwest - left 1 and down 1
                {
                    tile.x-=1;
                    tile.y+=1;
                }
                else
                {
                    cout << "***ERROR Processing input: " << tile.input << endl;
                }
            }
            else if (ch == CHAR_NORTH)
            {
                j++;
                ch = tile.input[j];
                if (ch == CHAR_EAST) // northeast - right 1 and up 1
                {
                    tile.x+=1;
                    tile.y-=1;
                }
                else if (ch == CHAR_WEST) // northweest - left 1 and up 1
                {
                    tile.x-=1;
                    tile.y-=1;
                }
                else
                {
                    cout << "***ERROR Processing input: " << tile.input << endl;
                }
            }
            else
            {
                cout << "***ERROR Processing input: " << tile.input << endl;
            }
        }
        cout << "Day24Tile " << tile.input << " finished at x=" << tile.x << " y=" << tile.y << endl;
        tiles.push_back(tile);
    }
    return tiles;
}

void AocDay24::determine_grid_sizes(vector<Day24Tile> & tiles, int buffer, int & min_x, int & max_x, int & min_y, int & max_y)
{
    cout << "Setting grid sizes" << endl;
    min_x=0;
    max_x=0;
    min_y=0;
    max_y=0;

    for (vector<Day24Tile>::iterator pos = tiles.begin(); pos != tiles.end(); ++pos)
    {
        int x = (*pos).x;
        int y = (*pos).y;
        if (x < min_x)
        {
            cout << " Day24Tile " << (*pos).input << " sets min_x to " << x << endl;
            min_x = x;
        }
        if (x > max_x)
        {
            cout << " Day24Tile " << (*pos).input << " sets max_x to " << x << endl;
            max_x = x;
        }
        if (y < min_y)
        {
            cout << " Day24Tile " << (*pos).input << " sets min_y to " << y << endl;
            min_y = y;
        }
        if (y > max_y)
        {
            cout << " Day24Tile " << (*pos).input << " sets max_y to " << y << endl;
            max_y = y;
        }
    }
    cout << "After going through the tiles  x:" << min_x << "-" << max_x << "  y:" << min_y << "-" << max_y << endl;
    if (abs(min_x) < max_x)
    {
        min_x = -1 * max_x;
    }
    else
    {
        max_x = -1 * min_x;
    }
    if (abs(min_y) < max_y)
    {
        min_y = -1 * max_y;
    }
    else
    {
        max_y = -1 * min_y;
    }
    cout << "After squaring off the region  x:" << min_x << "-" << max_x << "  y:" << min_y << "-" << max_y << endl;
    min_x-=buffer;
    max_x+=buffer;
    min_y-=buffer;
    max_y+=buffer;
    cout << "After applying buffer  x:" << min_x << "-" << max_x << "  y:" << min_y << "-" << max_y << endl;
    
    return;
}

void AocDay24::init_white_tiles(Screen & hex_grid, int min_x, int max_x, int min_y, int max_y)
{
    for (int y=min_y; y<=max_y; y++)
    {
        for (int x=min_x; x<=max_x; x++)
        {
            if (abs((y%2))==abs((x%2))) // shortcut - will set to white if both are odd or both are even. Will leave as blank otherwise
            {
                hex_grid.set(x,y,WHITE_TILE);
            }
        }
    }
}

void AocDay24::flip_tile(Screen & hex_grid, Day24Tile tile)
{
    char color = hex_grid.get(tile.x, tile.y);
    if (color == WHITE_TILE)
    {
        cout << "Flipping tile at x=" << tile.x << " y=" << tile.y << " to black" << endl;
        hex_grid.set(tile.x, tile.y, BLACK_TILE);
    }
    else if (color == BLACK_TILE)
    {
        cout << "Flipping tile at x=" << tile.x << " y=" << tile.y << " to white" << endl;
        hex_grid.set(tile.x, tile.y, WHITE_TILE);
    }
    else
    {
        cout << " Attempting to flip invalid location x=" << tile.x << " y=" << tile.y << endl;
    }
}

string AocDay24::part1(string filename, vector<string> extra_args)
{
    vector<string> lines = read_input(filename);
    int min_x, max_x, min_y, max_y;
    Screen * hex_grid = NULL;
    
    vector<Day24Tile> tiles = make_tiles(lines);
    
    determine_grid_sizes(tiles, PART1_BUFFER, min_x, max_x, min_y, max_y);
    
    hex_grid = new Screen(VOID, min_x, max_x, min_y, max_y);
    init_white_tiles(*hex_grid, min_x, max_x, min_y, max_y);
    
    hex_grid->display();
    for (vector<Day24Tile>::iterator pos = tiles.begin(); pos != tiles.end(); ++pos)
    {
        flip_tile(*hex_grid, *pos);
    }
    hex_grid->display();
    
    
    ostringstream out;
    out << hex_grid->num_matching(BLACK_TILE);
    delete hex_grid;
    return out.str();
}

