#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <algorithm>

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
