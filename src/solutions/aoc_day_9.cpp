#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

#include "aoc_day_9.h"
#include "file_utils.h"

using namespace std;

AocDay9::AocDay9():AocDay(9)
{
}

AocDay9::~AocDay9()
{
}

vector<string> AocDay9::read_input(string filename)
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

/*
* Compute all of the prior_sums for all elements.
    * Loop over each element from 1 to the end of the list with `i`
        * Loop backwards over the prior `preamble_length - 1` elements of the list with `j`
            * Store the sum of entries[i].value and entries[j].value in entries[i].prior_sums[i-j-1] (i.e. `n`+`n-1` is stored in `prior_sums[0]`; `n`+`n-2` is stored in `prior_sums[1]` etc)
*/
void AocDay9::compute_prior_sums(vector<Entry> & entries, int preamble_length)
{
#ifdef DEBUG_DAY9MATH
    cout << "Computing prior sums" << endl;
#endif
    for (int i = 1; i<entries.size(); i++)
    {
#ifdef DEBUG_DAY9MATH
        cout << "Computer for element " << i << " which has value " << entries[i].value << endl;
#endif
        for (int j = i - 1; (j >= 0) && (i - j < (preamble_length)); j--)
        {
#ifdef DEBUG_DAY9MATH
            cout << " Setting storing sum of " << i << "(" << entries[i].value << ") and " << j << "(" << entries[j].value << ") with value " << 
                    entries[i].value + entries[j].value << " in entries[" << i << "].prior_sums[" << i-j-1 << "]" << endl;
#endif
            entries[i].prior_sums[i-j-1]=entries[i].value + entries[j].value;
        }
    }
}

string AocDay9::part1(string filename, vector<string> extra_args)
{
    if (extra_args.size() != 1)
    {
        cerr << "Day 9 Part 1 required 1 extra argument for the preamble length)" << endl;
        return "";
    }
    
    int preamble_length = strtol(extra_args[0].c_str(), NULL, 10);
    cout << "preamble length is " << preamble_length << endl;
    vector<string> lines = read_input(filename);
    vector<struct Entry> entries;
    for (int i=0; i<lines.size(); i++)
    {
        Entry e;
        e.value=strtol(lines[i].c_str(), NULL, 10);
        entries.push_back(e);
    }
    
    compute_prior_sums(entries, preamble_length);
    
    long result = 0;
    
/*
* Start looping over `entries` at `preamble_length` with `n`
    * Set found = false
    * Loop from n-24 to n-1 with i
        * Loop from 0 to (i-(n-24)) with j
            * If `entries[i].prior_sums[j]` equals `entry[n].value`
                * Set found = true
                * Break from this loop
        * If found is true
            * Break from this loop
    * If not found
        * Return the not found value
*/
    
    for (int n=preamble_length; n < entries.size(); n++)
    {
        bool found = false;
#ifdef DEBUG_DAY9MATH
        cout << "Checking for element " << n << " which has value " << entries[n].value << endl;
#endif
        for (int i=n-(preamble_length-1); i <= n-1; i++)
        {
#ifdef DEBUG_DAY9MATH
            cout << " Checking prior sums in element " << i << endl;
#endif
            for (int j=0; j< (i-(n-preamble_length)); j++)
            {
#ifdef DEBUG_DAY9MATH
                cout << "  Checking entries[" << i << "].prior_sums[" << j << "] which has value " << entries[i].prior_sums[j] << endl;
#endif
                if (entries[n].value == entries[i].prior_sums[j])
                {
#ifdef DEBUG_DAY9MATH
                    cout << "  MATCHED" << endl;
#endif
                    found = true;
                    break;
                }
            }
            if (found)
            {
                break;
            }
        }
        if (!found)
        {
#ifdef DEBUG_DAY9MATH
            cout << " ELEMENT NOT FOUND; MUST BE ANSWER" << endl;
#endif
            result = entries[n].value;
            break;
        }
    }
    
    ostringstream out;
    out << result;
    return out.str();
}

string AocDay9::part2(string filename, vector<string> extra_args)
{
    if (extra_args.size() != 1)
    {
        cerr << "Day 9 Part 2 required 1 extra argument for the preamble length)" << endl;
        return "";
    }
    
    int preamble_length = strtol(extra_args[0].c_str(), NULL, 10);
    cout << "preamble length is " << preamble_length << endl;
    vector<string> lines = read_input(filename);
    vector<struct Entry> entries;
    for (int i=0; i<lines.size(); i++)
    {
        Entry e;
        e.value=strtol(lines[i].c_str(), NULL, 10);
        entries.push_back(e);
    }
    
    compute_prior_sums(entries, preamble_length);
    
    long not_found = 0;
    
    for (int n=preamble_length; n < entries.size(); n++)
    {
        bool found = false;
        for (int i=n-(preamble_length-1); i <= n-1; i++)
        {
            for (int j=0; j< (i-(n-preamble_length)); j++)
            {
                if (entries[n].value == entries[i].prior_sums[j])
                {
                    found = true;
                    break;
                }
            }
            if (found)
            {
                break;
            }
        }
        if (!found)
        {
            not_found = entries[n].value;
            break;
        }
    }
    
    cout << "Searching for not found value " << not_found << endl;
    
    long running_sum = entries[0].value;
    int start_position = 0;
    int end_position = 0;
    
/*
Loop to increase the range and adjust accordingly
* While running_sum does not equal the not_found value
    * While running_sum < not_found value
        * Increment end_position and add entries[end_position].value to running_sum
    * While running_sum > not_found value
        * Decrease running_sum by entries[start_position].value
        * Increment start_position
*/
    while (running_sum != not_found)
    {
        while (running_sum < not_found)
        {
            end_position++;
            running_sum+=entries[end_position].value;
        }
        while (running_sum > not_found)
        {
            running_sum-=entries[start_position].value;
            start_position++;
        }
    }
    
    cout << "Range from " << start_position << " to " << end_position << " equals sum " << running_sum << endl;
    
    long min=entries[start_position].value;
    long max=entries[start_position].value;
    
    for (int i=start_position+1; i<=end_position; i++)
    {
        if (entries[i].value < min)
        {
            min = entries[i].value;
        }
        if (entries[i].value > max)
        {
            max = entries[i].value;
        }
    }
    
    cout << "Max = " << max << " Min = " << min << " Sum = " << max+min << endl;
    
    ostringstream out;
    out << max+min;
    return out.str();
}