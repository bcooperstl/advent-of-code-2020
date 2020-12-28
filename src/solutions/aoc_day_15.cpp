#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <map>

#include "aoc_day_15.h"
#include "file_utils.h"

//#define DEBUG_DAY15

using namespace std;

AocDay15::AocDay15():AocDay(15)
{
}

AocDay15::~AocDay15()
{
}

vector<long> AocDay15::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    vector<vector<long>> split_longs;
    
    if (!fileutils.read_as_list_of_split_longs(filename, split_longs, ',', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return vector<long>();
    }
    return split_longs[0]; // return the first (and only) line
}

/* Planned approach
* Create a map<int, int> `last_turns` that maps a value to its last turn.
* Start a turn counter at 1.
* Initialize a value called prev_number
* Iterate over the starting numbers with value
    * If this is not the last starting number
        * Add/Update the `last_turns` map so that `last_turns[value]=turn_counter`
    * Increment the turn_counter
    * Set the prev_number to value
* While turn_counter <= 2020
    * If `last_turns[prev_number]` does not exist
        * Set last_turns[prev_number] to turn_counter-1
        * Set prev_number to 0
    * Else
        * Calculate new_value = `turn_counter - 1 - last_turns[prev_number]`
        * Set last_turns[prev_number] to turn_counter-1
        * Set prev_number=new_value
    * Increment the turn_counter
* Return prev_number as the solution
*/

string AocDay15::part1(string filename, vector<string> extra_args)
{
    vector<long> starting_numbers = read_input(filename);
    map<int, int> last_turns;
    int turn_counter = 1;
    int prev_number;
    
    for (int i=0; i<starting_numbers.size(); i++)
    {
        int value = starting_numbers[i];
        if (i != starting_numbers.size() -1)
        {
            last_turns[value]=turn_counter;
#ifdef DEBUG_DAY15
            cout << "Setting last_turns[" << value << "] to " << turn_counter << " by processing starting numbers" << endl;
#endif
        }
        turn_counter++;
        prev_number=value;
    }
    
    while (turn_counter <= 2020)
    {
        if (last_turns.find(prev_number) == last_turns.end())
        {
#ifdef DEBUG_DAY15
            cout << "Turn " << turn_counter << " has a new number. Setting last_turns[" << prev_number << "] to " << turn_counter-1 << " and prev_number to 0 " << endl;
#endif
            last_turns[prev_number] = turn_counter - 1;
            prev_number = 0;
        }
        else
        {
            int new_value = turn_counter - 1 - last_turns[prev_number];
#ifdef DEBUG_DAY15
            cout << "Turn " << turn_counter << " has a prior found number. Setting last_turns[" << prev_number << "] to " << turn_counter-1 << " and prev_number to " << new_value << endl;
#endif
            last_turns[prev_number] = turn_counter - 1;
            prev_number = new_value;
        }
        turn_counter++;
    }
    
    ostringstream out;
    out << prev_number;
    return out.str();
}

string AocDay15::part2(string filename, vector<string> extra_args)
{
    vector<long> starting_numbers = read_input(filename);
    int last_turns_low[1000000];
    for (int i=0; i<1000000; i++)
    {
        last_turns_low[i]=0;
    }
    
    map<int, int> last_turns;
    int turn_counter = 1;
    int prev_number;
    
    for (int i=0; i<starting_numbers.size(); i++)
    {
        int value = starting_numbers[i];
        // From data inspection, these are always less than 1000. Can just set in last_turns_low
        if (i != starting_numbers.size() -1)
        {
            last_turns_low[value]=turn_counter;
        }
        turn_counter++;
        prev_number=value;
    }
    
    while (turn_counter <= 30000000)
    {
        if (turn_counter%100000 == 0)
        {
            cout << turn_counter << endl;
        }
        
        if (prev_number < 1000000)
        {
            if (last_turns_low[prev_number] == 0)
            {
                last_turns_low[prev_number] = turn_counter - 1;
                prev_number = 0;
            }
            else
            {
                int new_value = turn_counter - 1 - last_turns_low[prev_number];
                last_turns_low[prev_number] = turn_counter - 1;
                prev_number = new_value;
            }
        }
        else
        {
            if (last_turns.find(prev_number) == last_turns.end())
            {
                last_turns[prev_number] = turn_counter - 1;
                prev_number = 0;
            }
            else
            {
                int new_value = turn_counter - 1 - last_turns[prev_number];
                last_turns[prev_number] = turn_counter - 1;
                prev_number = new_value;
            }
        }
        turn_counter++;
    }
    
    ostringstream out;
    out << prev_number;
    return out.str();
}
