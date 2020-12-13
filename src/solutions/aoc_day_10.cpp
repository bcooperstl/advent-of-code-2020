#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <bitset>

#include "aoc_day_10.h"
#include "file_utils.h"

using namespace std;

AocDay10::AocDay10():AocDay(10)
{
}

AocDay10::~AocDay10()
{
}

vector<long> AocDay10::read_input(string filename)
{
    FileUtils fileutils;
    vector<long> data;
    if (!fileutils.read_as_list_of_longs(filename, data))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return data;
    }
    return data;
}

/* planned approach
* Use the new FileUtils function to read in the input as a list of longs.
* Sort that list in ascending order.
* Increment for the first adapter by adding to the difference counter of going from 0 to it. (i.e if the first value is 2, increment the 2 counter)
* Walk the list, calculating the difference between each element and the one before it.
    * Increment counter buckets for the difference.
* Add one to the 3-jolt difference counter bucket to correspond to the device's internal step-up adapter.
* Return the number of 1-jolt differences multiplied by the number of 3-jolt differences.
*/

string AocDay10::part1(string filename, vector<string> extra_args)
{
    vector<long> data = read_input(filename);
    
    int difference_counters[4]={0,0,0,0}; // wasting a variable for the [0] just to make things clearer.
    
    sort(data.begin(), data.end());
    
    cout << "Incrementing the " << data[0] << " counter for the first step-up from 0" << endl;
    difference_counters[data[0]]++;
    
    // skip the first element since it doesn't have a prior element
    for (int i=1; i<data.size(); i++)
    {
        cout << "Going from " << data[i-1] << " to " << data[i] << " means incremeting " << data[i]-data[i-1] << endl;
        difference_counters[data[i]-data[i-1]]++;
    }
    
    cout << "Adding a final 3-level step up for the internal adapter" << endl;
    difference_counters[3]++;
    
    for (int i=1; i<=3; i++)
    {
        cout << "There are " << difference_counters[i] << " step ups for difference " << i << endl;
    }
    
    ostringstream out;
    out << difference_counters[1] * difference_counters[3];
    return out.str();
}

/* Planned approach
* Use the new FileUtils function to read in the input as a list of longs.
* Add the value 0 to that list to correspond to the input
* Sort that list in ascending order.

* Start a combination counter at 1. This will be a multiplied value
* Partition the vector into a set of vectors, splitting on cases where the difference between two neighboring values is 3.
* Walk through those partitions
    * Determine the number of combinations in each partion
    * Multiple the combination_counter by that value
* Return the final combination counter.
*/

long calculate_combinations(vector<long> values)
{
    long valid_combinations = 0;
    int length = values.size();
    
    cout << "Calcualting combinations for " << length << " elements in ";
    for (int i=0; i<values.size(); i++)
    {
        cout << values[i] << " ";
    }
    cout << endl;

    if (length == 1 || length == 2)
    {
        cout << " There is only one valid combination of a " << length << " sized list" << endl;
        return 1;
    }
    
#ifdef DEBUG_COMBINATIONS
    cout << "combinations must include the first element of " << values[0] << endl;
    cout << "combinations must include the last element of " << values[length-1] << endl;
#endif
    
    long combinations = 0x01l << length-2; //(that is 1 long shifted left by (count-2))
#ifdef DEBUG_COMBINATIONS
    cout << "There are " << combinations << " of the remaining " << length-2 << " elements " << endl;
#endif
    for (long indicators=0; indicators < combinations; indicators++)
    {
#ifdef DEBUG_COMBINATIONS
        cout << "indicators are " << indicators << " " << std::bitset<8>(indicators) << endl;
#endif
        long prev_value=values[0];
        bool good = true;
        for (int ind_pos=0; ind_pos < (length-2); ind_pos++)
        {
            if ((0x01l << ind_pos) && indicators)
            {
#ifdef DEBUG_COMBINATIONS
                cout << "including values[" << ind_pos + 1 << "] for position " << ind_pos << endl;
#endif
                long curr_value = values[ind_pos+1];
                if ((curr_value-3) > prev_value) // invalid - more than 3 above
                {
#ifdef DEBUG_COMBINATIONS
                    cout << " This is an invalid combiation - " << curr_value << " is more than 3 above " << prev_value << endl;
#endif
                    good = false;
                    break;
                }
                else
                {
#ifdef DEBUG_COMBINATIONS
                    cout << " Still a good combiation. Setting prev_value to curr_value of " << curr_value << endl;
#endif
                    prev_value = curr_value;
                }
            }
            else
            {
#ifdef DEBUG_COMBINATIONS
                cout << "skipping values[" << ind_pos + 1 << "] for position " << ind_pos << endl;
#endif
            }
        }
        if (good)
        {
            if (values[length-1] -3 > prev_value)
            {
#ifdef DEBUG_COMBINATIONS
                cout << " This is an invalid combiation - final " << values[length-1] << " is more than 3 above " << prev_value << endl;
#endif
                good = false;
            }
            else
            {
#ifdef DEBUG_COMBINATIONS
                cout << " Good combination - final value is 3 or less above " << prev_value << endl;
#endif
                valid_combinations++;
            }
        }
    }
    
    cout << " There are " << valid_combinations << " valid combinations from these elements" << endl;
    
    return valid_combinations;
}

string AocDay10::part2(string filename, vector<string> extra_args)
{
    vector<long> data = read_input(filename);
    data.push_back(0);
    
    sort(data.begin(), data.end());
    
    vector<vector<long>> partitions;
    vector<long>::iterator start = data.begin();
    vector<long>::iterator pos = next(start);
    
    long num_combiations = 1;
    
    while (pos != data.end())
    {
        long curr = *pos;
        long prev = *(next(pos, -1));
        
        if (curr == prev + 3)
        {
            cout << "Adding partition from values " << *start << "-" << prev << endl;
            partitions.push_back(vector<long>(start,pos)); // the vector constructor needs the element after the final one to add - hence pos, not prev(pos)
            start = pos;
        }
        ++pos;
    }
    cout << "Adding final partition from values " << *start << "-" << *(prev(pos)) << endl;
    partitions.push_back(vector<long>(start,pos));
    
    for (int i=0; i<partitions.size(); i++)
    {
        num_combiations *= calculate_combinations(partitions[i]);
    }
    
    ostringstream out;
    out << num_combiations;
    return out.str();
}
