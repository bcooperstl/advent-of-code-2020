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

int AocDay3::traverse_map(vector<string> map, int row_offset, int col_offset)
{
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
        row+=row_offset;
        col = ((col + col_offset) % width);
    }
    return trees;
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
    
    int trees = traverse_map(map, 1, 3); // down 1, right 3
    
    ostringstream out;
    out << trees;
    return out.str();
}

string AocDay3::part2(string filename, vector<string> extra_args)
{
/* Planned approach:
   * Read in the input as a list of strings as the map.
   * Set the tree product to 1
   * For each of the 5 slopes:
       * Call the traversal function with the map, row_increment and column_increment
       * Muliple the tree_product by the result of that function.
   * Return the tree product.
*/

    vector<string> map = read_input(filename);
    
    long trees;
    long tree_product = 1;
    
    trees = traverse_map(map, 1, 1);
    cout << "Right 1, down 1 has " << trees << " trees" << endl;
    tree_product *= trees;
    
    trees = traverse_map(map, 1, 3);
    cout << "Right 3, down 1 has " << trees << " trees" << endl;
    tree_product *= trees;
    
    trees = traverse_map(map, 1, 5);
    cout << "Right 5, down 1 has " << trees << " trees" << endl;
    tree_product *= trees;
    
    trees = traverse_map(map, 1, 7);
    cout << "Right 7, down 1 has " << trees << " trees" << endl;
    tree_product *= trees;
    
    trees = traverse_map(map, 2, 1);
    cout << "Right 1, down 2 has " << trees << " trees" << endl;
    tree_product *= trees;
    
    ostringstream out;
    out << tree_product;
    return out.str();
}
