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

regex AocDay16::field_regex("([\\w ]+): (\\d+)-(\\d+) or (\\d+)-(\\d+)");

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
                for (int field_idx=0; field_idx <input.fields.size(); field_idx++)
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


/* Planned approach
* Create a map<int, bool> valid_values to store my cache.
* Create a vector<Ticket> valid_tickets
* Loop over the list of nearby_tickets with ticket
    * Set a bool ticket_valid to true
    * For each value in ticket
        * If the value is in the valid_values cache
            * If the cache shows it false (invalid)
                * set ticket_valid to false
                * break out of the loop
        * Else (the value is not in the cache)
            * Set `found` to false
            * For each field
                * If the value is valid for the field
                    * Set `found` to true
                    * Stop looking at other fields
            * Store the value and found into the valid_values cache
            * If found is false
                * set ticket_valid to false
                * break out of the loop
    * If ticket_valid is true
        * Add the ticket to the valid_tickets vector
* Return the valid_tickets vector
*/

vector<Ticket> AocDay16::remove_invalid_tickets(Day16Input & input)
{
    map<int, bool> valid_values_cache;
    vector<Ticket> valid_tickets;
    
    for (int ticket_idx=0; ticket_idx<input.nearby_tickets.size(); ticket_idx++)
    {
        bool ticket_valid = true;
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
                    cout << "   Cache shows false; Dropping ticket" << endl;
#endif
                    ticket_valid = false;
                    break;
                }
            }
            else
            {
#ifdef DEBUG_DAY16
                cout << "  Cache Miss" << endl;
#endif
                bool found = false;
                for (int field_idx=0; field_idx <input.fields.size(); field_idx++)
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
                if (!found)
                {
#ifdef DEBUG_DAY16
                    cout << "   Not found in any field. Dropping ticket" << endl;
#endif
                    ticket_valid = false;
                    break;
                }
            }
        }
        if (ticket_valid)
        {
#ifdef DEBUG_DAY16
                    cout << " Ticket is valid" << endl;
#endif
            valid_tickets.push_back(input.nearby_tickets[ticket_idx]);
        }
    }
    return valid_tickets;
}

void AocDay16::populate_field_position_possibilities(Possibilities & possibilities, vector<Ticket> &valid_tickets, vector<Field> &fields)
{
    cout << "Initializing field_position_possibilities" << endl;
    for (int i=0; i<possibilities.num_fields; i++)
    {
        for (int j=0; j<possibilities.num_fields; j++)
        {
            possibilities.field_position_possibilities[i][j]=true;
        }
    }
    
    display_field_position_possibilities(possibilities, "After initialization");
    
    int ticket_idx=0;
    for (vector<Ticket>::iterator ticket_iter=valid_tickets.begin(); ticket_iter != valid_tickets.end(); ++ticket_iter, ++ticket_idx)
    {
        Ticket ticket = *ticket_iter;
        for (int val_idx=0; val_idx < possibilities.num_fields; val_idx++)
        {
            for (int field_idx=0; field_idx < possibilities.num_fields; field_idx++)
            {
                if ((!fields[field_idx].is_valid(ticket.values[val_idx])) && (possibilities.field_position_possibilities[field_idx][val_idx]))
                {
#ifdef DEBUG_DAY16
                    cout << " Setting field " << field_idx << " (" << fields[field_idx].get_name() 
                         << ") to invlaid due to value " << ticket.values[val_idx] 
                         << " at position " << val_idx << " in ticket " << ticket_idx << endl;
#endif
                    possibilities.field_position_possibilities[field_idx][val_idx]=false;
                }
            }
        }
    }

    display_field_position_possibilities(possibilities, "After going through valid tickets");
}

/* Planned approach
* create boolean arrays fields_done[MAX_NUM_FIELDS] and positions_done[MAX_NUM_FIELDS], with all values set to false.
* clear the map<int, int> fields_to_position_map
* set work_done to false
* Do
    * loop over the fields with field_idx // doing fields first
        * if fields_done[field_idx] is false // haven't solved this field yet
            * count the number of true values in field_position_possibilities[field_idx][n]
            * If the count is 1 // this means the field is solved
                * set fields_done[field_idx] to true
                * store the matching position index for the one true value in position_idx
                * set positions_done[position_idx] to true
                * set all other field_position_possibilities[n][position_idx] to false. // This will make sure it is not counted as a possibility for any other fields
                * set fields_to_position_map[field_idx]=position_idx
                * set work_done to true // we did something; so try again to further refine the solution the next time through
    * loop over the positions with position_idx // doing positions second
        * if positions_done[position_idx] is false // haven't solved this position yet
            * count the number of true values in field_position_possibilities[n][position_idx]
            * If the count is 1 // this means the position is solved
                * set positions_done[position_idx] to true
                * store the matching field index for the one true value in field_idx
                * set fields_done[field_idx] to true
                * set all other field_position_possibilities[field_idx][n] to false. // This will make sure it is not counted as a possibility for any other positions
                * set fields_to_position_map[field_idx]=position_idx
                * set work_done to true // we did something; so try again to further refine the solution the next time through
* while (work_done == true)
*/

void AocDay16::reduce_field_position_possibilities(Possibilities & possibilities, map<int, int> & fields_to_position_map)
{
    bool fields_done[MAX_NUM_FIELDS];
    bool positions_done[MAX_NUM_FIELDS];
    for (int i=0; i<possibilities.num_fields; i++)
    {
        fields_done[i]=false;
        positions_done[i]=false;
    }
    fields_to_position_map.clear();
    bool work_done = false;
    
    display_field_position_possibilities(possibilities, "Prior to first reduce iteration");
    int iteration_counter=1;
    
    do
    {
        work_done = false;
        cout << "Iteration " << iteration_counter << endl;
        // fields first
        for (int field_idx=0; field_idx<possibilities.num_fields; field_idx++)
        {
            if (fields_done[field_idx]) // no need to deal with this one if it is done
            {
#ifdef DEBUG_DAY16
                cout << " Skipping field " << field_idx << " because it is already processed" << endl;
#endif
                continue;
            }
            
#ifdef DEBUG_DAY16
            cout << " Processing field " << field_idx << endl;
#endif
            int possible_positions_for_field=0;
            int matching_position_idx=0;
            for (int i=0; i<possibilities.num_fields; i++)
            {
                if (possibilities.field_position_possibilities[field_idx][i] == true)
                {
                    possible_positions_for_field++;
                    matching_position_idx=i;
                }
            }
#ifdef DEBUG_DAY16
            cout << "  There are " << possible_positions_for_field << " possible positions for this field" << endl;
#endif
            if (possible_positions_for_field == 1)
            {
#ifdef DEBUG_DAY16
                cout << "  Setting fields_done[" << field_idx << "] and positions_done[" 
                     << matching_position_idx << "] to true" << endl;
#endif
                fields_done[field_idx]=true;
                positions_done[matching_position_idx]=true;
                for (int unset_field_idx=0; unset_field_idx<possibilities.num_fields; unset_field_idx++)
                {
                    if (unset_field_idx!=field_idx && possibilities.field_position_possibilities[unset_field_idx][matching_position_idx])
                    {
#ifdef DEBUG_DAY16
                        cout << "  Setting field_position_possibilities[" << unset_field_idx << "][" 
                             << matching_position_idx << "] to false since position " << matching_position_idx 
                             << " is now set" << endl;
#endif
                        possibilities.field_position_possibilities[unset_field_idx][matching_position_idx] = false;
                    }
                }
                cout << " Matching field " << field_idx << " to position " << matching_position_idx << endl;
                fields_to_position_map[field_idx]=matching_position_idx;
                work_done = true;
                display_field_position_possibilities(possibilities);
            }
        }
        
        // positions second
        for (int position_idx=0; position_idx<possibilities.num_fields; position_idx++)
        {
            if (positions_done[position_idx]) // no need to deal with this one if it is done
            {
#ifdef DEBUG_DAY16
                cout << " Skipping position " << position_idx << " because it is already processed" << endl;
#endif
                continue;
            }
            
#ifdef DEBUG_DAY16
            cout << " Processing position " << position_idx << endl;
#endif
            int possible_fields_for_position=0;
            int matching_field_idx=0;
            for (int i=0; i<possibilities.num_fields; i++)
            {
                if (possibilities.field_position_possibilities[i][position_idx] == true)
                {
                    possible_fields_for_position++;
                    matching_field_idx=i;
                }
            }
#ifdef DEBUG_DAY16
            cout << "  There are " << possible_fields_for_position << " possible fields for this position" << endl;
#endif
            if (possible_fields_for_position == 1)
            {
#ifdef DEBUG_DAY16
                cout << "  Setting positions_done[" << position_idx << "] and fields_done[" 
                     << matching_field_idx << "] to true" << endl;
#endif
                positions_done[position_idx]=true;
                fields_done[matching_field_idx]=true;
                for (int unset_position_idx=0; unset_position_idx<possibilities.num_fields; unset_position_idx++)
                {
                    if (unset_position_idx!=position_idx && possibilities.field_position_possibilities[matching_field_idx][unset_position_idx])
                    {
#ifdef DEBUG_DAY16
                        cout << "  Setting field_position_possibilities[" << matching_field_idx << "][" 
                             << unset_position_idx << "] to false since field " << matching_field_idx 
                             << " is now set" << endl;
#endif
                        possibilities.field_position_possibilities[matching_field_idx][unset_position_idx] = false;
                    }
                }
                cout << " Matching field " << matching_field_idx << " to position " << position_idx << endl;
                fields_to_position_map[matching_field_idx]=position_idx;
                work_done = true;
                display_field_position_possibilities(possibilities);
            }
        }

        
        iteration_counter++;
        display_field_position_possibilities(possibilities, "After complete iteration");
    } while (work_done);
}

/* Planned approach
* Parse the intput as described above.
* Remove the invalid tickets as described above.
* Create the field_position_possibilities array as all true values
* Create an empty map<int, int> for fields_to_position_map
* Call the function to populate the field_position_possibilities array.
* Call the function to reduce the field_position_possibilities array, also giving it the fields_to_position_map.
* Initialize a product value to 1
* Loop over the list of fields with field_idx.
    * If fields[field_idx] start with "departure"
        * Look up the value position in fields_to_position_map and store in position
        * Multiply the product value by your_ticket.values[position]
* Return the product
*/

string AocDay16::part2(string filename, vector<string> extra_args)
{
    Day16Input input;
    map<int, int> fields_to_position_map;
    Possibilities possibilities;
    long long product = 1;
    
    parse_input(filename, input);
    
    vector<Ticket> valid_tickets = remove_invalid_tickets(input);
                    
    possibilities.num_fields=input.fields.size();
    populate_field_position_possibilities(possibilities, valid_tickets, input.fields);
    reduce_field_position_possibilities(possibilities, fields_to_position_map);
    
    for (map<int, int>::iterator map_iter = fields_to_position_map.begin(); map_iter != fields_to_position_map.end(); ++map_iter)
    {
        cout << "Field " << map_iter->first << "(" << input.fields[map_iter->first].get_name() << ") corresponds to position " << map_iter->second << endl;
    }
    
    cout << "Computing final result" << endl;
    for (int i=0; i<possibilities.num_fields; i++)
    {
        if (input.fields[i].get_name().find("departure") == 0)
        {
            int position = fields_to_position_map[i];
            long long value = input.your_ticket.values[position];
            cout << " Field " << input.fields[i].get_name() << " is at position " << position << " and your_ticket has value " << value << endl;
            product *= value;
        }
    }    
    
    cout << endl;
    
    ostringstream out;
    out << product;
    return out.str();
}





/*
    value:  0   1   2
field:
        0   T   F   T
        1   F   T   F
        2   T   F   F
*/

void AocDay16::display_field_position_possibilities(Possibilities & possibilities)
{
    display_field_position_possibilities(possibilities, "");
}

void AocDay16::display_field_position_possibilities(Possibilities & possibilities, string heading)
{
    cout << heading << endl;
    cout << "\tPos:";
    for (int i=0; i<possibilities.num_fields; i++)
    {
        cout << "\t" << i;
    }
    cout << endl;
    cout << "field:" << endl;
    for (int i=0; i<possibilities.num_fields; i++)
    {
        cout << "\t" << i;
        for (int j=0; j<possibilities.num_fields; j++)
        {
            cout << "\t" << (possibilities.field_position_possibilities[i][j] ? 'T' : 'F');
        }
        cout << endl;
    }
}

