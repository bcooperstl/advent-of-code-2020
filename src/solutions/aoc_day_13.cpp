#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_13.h"
#include "file_utils.h"

using namespace std;

AocDay13::AocDay13():AocDay(13)
{
}

AocDay13::~AocDay13()
{
}

void AocDay13::parse_input_part_1(string filename, long & timestamp, vector<long> & bus_ids)
{
    FileUtils fileutils;
    vector<vector<string>> split_lines;
    if (!fileutils.read_as_list_of_split_strings(filename, split_lines, ',', '\0', '\0'))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
    
    timestamp = strtol(split_lines[0][0].c_str(), NULL, 10);
    cout << "Timestamp is " << timestamp << endl;
    vector<string> bus_id_strings = split_lines[1];
    for (vector<string>::iterator id_iter = bus_id_strings.begin(); id_iter != bus_id_strings.end(); ++id_iter)
    {
        string bus_id = *id_iter;
        if (bus_id[0]=='x')
        {
            continue;
        }
        cout << "Adding bus_id " << bus_id << endl;
        bus_ids.push_back(strtol(bus_id.c_str(), NULL, 10));
    }
    return;
}

/* planned approach
* Parse the input as described above, returning the start_time and a list of Bus_IDs
* Set result_bus_id and result_time variables to 0.
* For each Bus ID:
    * Calculate how many minutes past the last bus the start time is by by doing `start_time mod bus_id`. Store this in `minutes_past_last`.
    * Calculate the next time that bus arrives by `start_time - minutes_past_last + bus_id` and storing that in `next_time`.
    * if `result_bus_id` is `0 or `next_time < result_time`
        * Set `result_bus_id` and `result_time` to this `bus_id` and `next_time` values
* Return `result_bus_id * (result_time - start_time)`

*/

string AocDay13::part1(string filename, vector<string> extra_args)
{
    vector<long> bus_ids;
    long start_timestamp;
    parse_input_part_1(filename, start_timestamp, bus_ids);
    
    long result_bus_id=0;
    long result_timestamp=0;
    
    for (vector<long>::iterator bus_id_iter = bus_ids.begin(); bus_id_iter != bus_ids.end(); ++bus_id_iter)
    {
        long bus_id = *bus_id_iter;
        long minutes_past_last = start_timestamp % bus_id;
        long next_time = start_timestamp - minutes_past_last + bus_id;
        cout << "Bus ID " << bus_id << " arrives at " << next_time << endl;
        if (result_bus_id == 0 || next_time < result_timestamp)
        {
            cout << " This is the new best time" << endl;
            result_bus_id = bus_id;
            result_timestamp = next_time;
        }
    }
    
    cout << endl;
    
    ostringstream out;
    out << (result_bus_id * (result_timestamp - start_timestamp));
    return out.str();
}
