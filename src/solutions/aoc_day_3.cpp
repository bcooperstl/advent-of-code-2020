#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

#include "aoc_day_3.h"
#include "file_utils.h"

using namespace std;

#define TREE '#'
#define OPEN_SPACE '.'

AocDay3::AocDay3():AocDay(3)
{
}

AocDay3::~AocDay3()
{
}

vector<string> AocDay3::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    vector<long> data;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return raw_lines;
}

string AocDay3::part1(string filename, vector<string> extra_args)
{
/* Planned approach:
   * Read in the input as a list of strings as the map.
   * Get the height of the area by the number of strings in the map.
   * Get the width of the area by the string length of the first string in the map.
   * Define my start coordinate as row=0, column=0
   * While row is less than height
       * Check the (row,column) position to see if there is a tree or not, and increment the trees counter if a tree is found
       * Increment row by 1 to move down 1
       * Increment column by 3 to move right 3. Since this could go off the right of the map, use the modulo this by the width to bring it back onto the map. This eliminates the needs to make copies of the map to the right
   * Return the number of trees
*/
    
    vector<string> map = read_input(filename);
    
    int height = map.size();
    int width = map[0].length();
    int row = 0;
    int col = 0;
    int trees = 0;
    
    while (row < height)
    {
        if (map[row][col] == TREE)
        {
            trees++;
        }
        row++;
        col = ((col + 3) % width);
    }
    
    ostringstream out;
    out << trees;
    return out.str();
}
