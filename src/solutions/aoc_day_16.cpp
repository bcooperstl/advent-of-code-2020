#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>

#include "aoc_day_16.h"
#include "file_utils.h"

#define YOUR_TICKET "your ticket:"
#define NEARBY_TICKETS "nearby tickets:"

//#define DEBUG_REGEX
#define DEBUG_DAY16

using namespace std;

Field::Field(string name)
{
    m_name=name;
}

Field::~Field()
{
}

void Field::add_valid_value_range(int low, int high)
{
    m_valid_value_ranges.push_back(pair<int, int>(low, high));
}

string Field::get_name()
{
    return m_name;
}

bool Field::is_valid(int target)
{
    for (int i=0; i<m_valid_value_ranges.size(); i++)
    {
        if (m_valid_value_ranges[i].first <= target && m_valid_value_ranges[i].second >= target)
        {
            return true;
        }
    }
    return false;
}

regex AocDay16::field_regex("(\\w+): (\\d+)-(\\d+) or (\\d+)-(\\d+)");

AocDay16::AocDay16():AocDay(16)
{
}

AocDay16::~AocDay16()
{
}

/* Planned approach to input parsing:
There are three sections to the input file to parse.
1) Fields
2) Your Ticket
3) Nearby Tickets

There are blank lines between the sections.

To parse the fields, loop until the first blank line is reached.  
Use a regex `(\w+): (\d+)-(\d+) or (\d+)-(\d+)` top split out the field name and the ranges. Every element has 2 ranges, so this will work for all of them.

Skip the "your ticket:" label line
Create a FileUtils split_line_to_ints function that works similar to the split_line_to_strings function to read the value lines.

Skip the blank line and the "nearby ticket:" label line.
While there are still lines in the file
    Use the same FileUtils split_line_to_ints function to read each nearby ticket values.
*/

void AocDay16::parse_input(string filename, Day16Input & input)
{
    input.fields.clear();
    input.nearby_tickets.clear();
    
    FileUtils fileutils;
    vector<string> lines;
    if (!fileutils.read_as_list_of_strings(filename, lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return;
    }
    
    vector<string>::iterator line_pos = lines.begin();
    // section 1 - fields
    while (*line_pos != "")
    {
        string line = *line_pos;
#ifdef DEBUG_REGEX
        cout << "Field Regex - Processing line [" << line << "]" << endl;
#endif
        match_results<std::string::const_iterator> matches;
    
        // First search to see if it is a mask line to see if this matches a line with no contents
        regex_search(line, matches, field_regex);
        if (matches.size() > 0)
        {
#ifdef DEBUG_REGEX
            cout << " Field Regex matches for field " << matches[1] << endl;
#endif
            Field field(matches[1].str());
            for (int i=0; i<2; i++)
            {
                int low = strtol(matches[2+(i*2)].str().c_str(), NULL, 10);
                int high = strtol(matches[2+(i*2)+1].str().c_str(), NULL, 10);
#ifdef DEBUG_REGEX
                cout << " Adding range from " << low << " to " << high << endl;
#endif
                field.add_valid_value_range(low, high);
            }
            input.fields.push_back(field);
        }
        else
        {
            // Not a mask line. Search to see if it is a mem line
#ifdef DEBUG_REGEX
            cout << " *****Field Regex does not match! Fix it!!" << endl;
#endif
        }
        ++line_pos;
    }
    
    // At the blank line. Increment one for the YOUR_TICKET label and then another to get to your ticket
    ++line_pos;
    if (*line_pos != YOUR_TICKET)
    {
        cerr << "PARSING MISMATCH - Expected " << YOUR_TICKET << " Got:" << *line_pos << endl;
    }
    ++line_pos;
    
    input.your_ticket.values=fileutils.split_line_to_ints(*line_pos, ',');
    
    // Increment once for the blank line, once for the NEARBY_TICKETS label, and one more time to get to the first nearby ticket.
    ++line_pos; // now at blank line
    ++line_pos; // now at NEARBY_TICKETS label.
    if (*line_pos != NEARBY_TICKETS)
    {
        cerr << "PARSING MISMATCH - Expected " << NEARBY_TICKETS << " Got:" << *line_pos << endl;
    }
    
    ++line_pos;// Now at first nearby ticket.

    while (line_pos != lines.end())
    {
        Ticket nearby_ticket;
        nearby_ticket.values=fileutils.split_line_to_ints(*line_pos, ',');
        input.nearby_tickets.push_back(nearby_ticket);
        ++line_pos;
    }
    return;
}


/* planned approach
* Parse the intput as described above.
* Initialize the error_rate_sum to 0.
* Create a map<int, bool> valid_values to store my cache.
* Loop over the list of nearby_tickets with ticket
    * For each value in ticket
        * If the value is in the valid_values cache
            * If the cache shows it false (invalid)
                * Add the value to error_rate_sum
            * Continue to the next value
        * Else (the value is not in the cache)
            * Set `found` to false
            * For each field
                * If the value is valid for the field
                    * Set `found` to true
                    * Stop looking at other fields
            * If found is false
                * Add the value to error_rate_sum
            * Store the value and found into the valid_values cache
* Return the error_rate_sum

*/

string AocDay16::part1(string filename, vector<string> extra_args)
{
    Day16Input input;
    int error_rate_sum = 0;
    map<int, bool> valid_values_cache;
    
    parse_input(filename, input);
    
    for (int ticket_idx=0; ticket_idx<input.nearby_tickets.size(); ticket_idx++)
    {
#ifdef DEBUG_DAY16
        cout << "Processing ticket " << ticket_idx << endl;
#endif
        for (int value_idx=0; value_idx<input.nearby_tickets[ticket_idx].values.size(); value_idx++)
        {
            int value = input.nearby_tickets[ticket_idx].values[value_idx];
#ifdef DEBUG_DAY16
            cout << " Processing value " << value << endl;
#endif
            map<int, bool>::iterator cache_lookup = valid_values_cache.find(value);
            if (cache_lookup != valid_values_cache.end())
            {
#ifdef DEBUG_DAY16
                cout << "  Cache Hit" << endl;
#endif
                if (!(cache_lookup->second))
                {
#ifdef DEBUG_DAY16
                    cout << "   Cache shows false; Adding to error rate sum" << endl;
#endif
                    error_rate_sum += value;
                }
                continue;
            }
            else
            {
#ifdef DEBUG_DAY16
                cout << "  Cache Miss" << endl;
#endif
                bool found = false;
                for (int field_idx=0; field_idx <=input.fields.size(); field_idx++)
                {
                    if (input.fields[field_idx].is_valid(value))
                    {
#ifdef DEBUG_DAY16
                        cout << "  Value " << value << " Found in field " << input.fields[field_idx].get_name() << endl;
#endif
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
#ifdef DEBUG_DAY16
                    cout << "   Not found in any field. Adding to error rate sum" << endl;
#endif
                    error_rate_sum += value;
                }
#ifdef DEBUG_DAY16
                cout << "   Adding " << value << " to cache as " ;
                if (found)
                {
                    cout << " true" << endl;
                }
                else
                {
                    cout << " false " << endl;
                }
#endif
                valid_values_cache[value]=found;
            }
        }
    }
                    
    cout << endl;
    
    ostringstream out;
    out << error_rate_sum;
    return out.str();
}
