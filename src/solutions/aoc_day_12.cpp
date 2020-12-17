#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_12.h"
#include "file_utils.h"

using namespace std;

#define NORTH 'N'
#define SOUTH 'S'
#define EAST 'E'
#define WEST 'W'
#define LEFT 'L'
#define RIGHT 'R'
#define FORWARD 'F'

AocDay12::AocDay12():AocDay(12)
{
}

AocDay12::~AocDay12()
{
}

vector<Day12Instruction> AocDay12::parse_input(string filename)
{
    FileUtils fileutils;
    vector<string> lines;
    vector<Day12Instruction> instructions;
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return instructions;
    }
    
    for (vector<string>::iterator line_iter = lines.begin(); line_iter != lines.end(); ++line_iter)
    {
        string line = *line_iter;
        Day12Instruction inst;
        inst.action=line[0];
        inst.value=strtol(line.c_str()+1, NULL, 10); // skip the first character that was the action
        instructions.push_back(inst);
    }
    return instructions;
}

/* planned approach
* Parse the Input as described above
* Set the current_direction to 0 to correspond to `E` in the directions array
* Set the current_x and current_y to 0 for them to be at the origin
* Loop over the Instructions
    * Store the Instruction's Action in current_action
    * If the Instruction's Action is `F` for forward:
        * Overwrite the current_action with the directions[current_direction] value.
    * If the current_action is `N`, `S`, `E`, or `W` (Note this also captures the Forward case)
        * Move the ship by adding or subtracting the Value to current_x or current_y as appropriate
    * If the current_action is 'L' or 'R'
        * Change the current direction by adding (if right) or subtracting (if left) the value/90 to the current_direction **AND ADDING 4** and modding it by 4. This will result in a value between 0 and 3
* Return the Manhattan distance from the origin. Since I started at 0,0, it's just `(abs(current_x) + abs(current_y))`
*/

string AocDay12::part1(string filename, vector<string> extra_args)
{
    vector<Day12Instruction> instructions = parse_input(filename);
    
    char directions[4]={EAST, SOUTH, WEST, NORTH}; // start a 0
    int current_direction_index=0; // Start at 0 for East
    int current_x = 0; // start at the origin
    int current_y = 0;
    
    cout << "Starting at (" << current_x << "," << current_y << ") facing " << directions[current_direction_index] << endl;
    
    for (vector<Day12Instruction>::iterator inst_iter = instructions.begin(); inst_iter != instructions.end(); ++inst_iter)
    {
        Day12Instruction inst = *inst_iter;
        char current_action = inst.action;
        if (current_action == FORWARD)
        {
            current_action = directions[current_direction_index];
        }
        
        if (current_action == NORTH)
        {
            current_y += inst.value;
        }
        else if (current_action == SOUTH)
        {
            current_y -= inst.value;
        }
        else if (current_action == EAST)
        {
            current_x += inst.value;
        }
        else if (current_action == WEST)
        {
            current_x -= inst.value;
        }
        else if (current_action == RIGHT)
        {
            current_direction_index = ((current_direction_index + (inst.value/90)) % 4); // always adding a value that is 90, 180, or 270
        }
        else if (current_action == LEFT)
        {
            current_direction_index = ((current_direction_index - (inst.value/90) + 4) % 4); // subtracint from a value that is 90, 180, or 270, so need to add 4 for the module to stay positive
        }
        cout << "Instruction " << inst.action << " " << inst.value << " results in position (" << current_x << "," << current_y << ") facing " << directions[current_direction_index] << endl;
    }
    
    cout << endl;
    
    ostringstream out;
    out << (abs(current_x)+abs(current_y));
    return out.str();
}
