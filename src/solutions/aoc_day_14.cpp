#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdint>
#include <regex>

#include "aoc_day_14.h"
#include "file_utils.h"

#define INIT_ZERO_MASK 0x0000000fffffffffULL // all ones. bits that are 0 will be set to 0
#define INIT_ONES_MASK 0x0000000000000000ULL // all zeros. bits that are 1 will be set to 1
#define ONE_BIT 0x01ULL
#define ZERO_BIT 0x00ULL

using namespace std;

regex AocDay14::mask_regex("^mask = ([01X]{36})$");
regex AocDay14::mem_regex("^mem\\[(\\d+)\\] = (\\d+)$");

AocDay14::AocDay14():AocDay(14)
{
}

AocDay14::~AocDay14()
{
}

vector<string> AocDay14::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return raw_lines;
}

void AocDay14::setup_mask_values(string mask_str, uint64_t & zero_mask, uint64_t & ones_mask)
{
    // Zero mask starts as all 1s, 
    zero_mask = INIT_ZERO_MASK;
    ones_mask = INIT_ONES_MASK; // start as all zeros
    
    for (int i=0; i<36; i++)
    {
        // one bit set i positions from the end. set the ones_mask to 1 in the corresponding bit
        if (mask_str[35-i] == '1')
        {
            ones_mask += (ONE_BIT << i);
        }
        if (mask_str[35-i] == '0')
        {
            zero_mask -= (ONE_BIT << i);
        }
    }
    cout << "Mask " << mask_str << " results in " << endl << " zero_mask " << std::bitset<32>(zero_mask) << endl << " ones_mask " << std::bitset<32>(ones_mask) << endl;
}

/*
Use the lookup table find the shiny gold Bag. From this, go through each member of the contained_by list. Mark them processed and add them to a list of container bags.  
Iterate through this container bags list, doing the same lookup and add to the end of list, but only if the looked up bag has not been processed.  
Repeat until every element in the list has been done.  
The length of this list is the result.
*/
string AocDay14::part1(string filename, vector<string> extra_args)
{
    vector<string> lines = read_input(filename);
    map<uint64_t, uint64_t> memory;
    string mask;
    uint64_t ones_mask, zero_mask;
    
    for (vector<string>::iterator iter = lines.begin(); iter != lines.end(); ++iter)
    {
        string line = *iter;
        
#ifdef DEBUG_REGEX
        cout << "Processing line [" << line << "]" << endl;
#endif
        match_results<std::string::const_iterator> matches;
    
        // First search to see if it is a mask line to see if this matches a line with no contents
        regex_search(line, matches, mask_regex);
        if (matches.size() > 0)
        {
#ifdef DEBUG_REGEX
            cout << " Mask line matches with mask " << matches[1] << endl;
#endif
            mask = matches[1].str();
            setup_mask_values(mask, zero_mask, ones_mask);
        }
        else
        {
            // Not a mask line. Search to see if it is a mem line
#ifdef DEBUG_REGEX
            cout << " Mask line does not match. Trying mem line." << endl;
#endif
            regex_search(line, matches, mem_regex);
            if (matches.size() > 0)
            {
#ifdef DEBUG_REGEX
                cout << "  Mem line matches with address " << matches[1] << " and value " << matches[2] << endl;
#endif
                uint64_t address = strtoull(matches[1].str().c_str(), NULL, 10);
                uint64_t initial_value = strtoull(matches[2].str().c_str(), NULL, 10);
            
                uint64_t final_value = (initial_value | ones_mask) & zero_mask;
        
                cout << "Initial value " << initial_value << " results in final value " << final_value << ". Storing in address " << address << endl;
                memory[address] = final_value;
            }
            else
            {   
                cout << "NO MATCHING REGEX FOR LINE[" << line << "]" << endl;
            }
        }
    }

    uint64_t total = 0;
    for (map<uint64_t, uint64_t>::iterator mem_iter = memory.begin(); mem_iter != memory.end(); ++mem_iter)
    {
        total+=mem_iter->second;
    }
    
    ostringstream out;
    out << total;
    return out.str();
}
