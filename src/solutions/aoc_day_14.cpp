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

#define INIT_SET_ONES_MASK 0x0000000000000000ULL // all zeros. bits that are 1 will be set to 1
#define INIT_CLEAR_X_MASK 0x0000000fffffffffULL // all ones. bits that are X will be set to 0
#define FIRST_X_MASK 0x00ULL // start with a 0

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

void AocDay14::setup_value_mask_values(string mask_str, uint64_t & zero_mask, uint64_t & ones_mask)
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
* Parse the input into a list of strings.
* Create an empty map for memory.
* Loop over each line
    * If the line matches the mask regex:
        * Extract the mask from the capture group.
        * Calculate the one_mask and zero_mask based on the extracted match.
    * If the line matches the mem regex:
        * Extract the address and value from the capture groups.
        * Apply the current one_mask and zero_mask to the value, and store that result in the memory map for the address.
    * Else
        * Throw an error - did something wrong :-)
* Loop over the map, summing the values and return that sum.

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
            setup_value_mask_values(mask, zero_mask, ones_mask);
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

/* From documentation:
When a new mask value is given, we will create several masks.

The first one is a set_ones_mask that will be used to set all of the `1` bits to 1. It will consist of `1` values in all of those bit positions, and `0` values for the `0` and `X` bits.

The second one is a clear_x_mask that will be used to set all of the `X` bits to 0. It will consist of `0` values in all of those bit positions, and `1` values for the `0` and `1` bits.

There will then be a list of x_masks created to apply the different X permutations.

* Create a list of x_masks that initially the value 0. This list will contain all of the values to OR the base_address against.
* For every `X` bit in the mask
    * For every item in the x_masks list
        * Append a new copy of that item, with the position of the current `X` bit set to `1`. This will double the size of the list each time.
*/

void AocDay14::setup_address_mask_values(string mask_str, uint64_t & set_ones_mask, uint64_t & clear_x_mask, vector<uint64_t> & x_masks)
{
#ifdef DEBUG_DAY_14
    cout << "Setting up address mask values" << endl;
#endif
    set_ones_mask = INIT_SET_ONES_MASK;  // start with all zeros and set the ones
    clear_x_mask = INIT_CLEAR_X_MASK;    // start with all ones and set the zeroes
    
    x_masks.clear();
    x_masks.push_back(FIRST_X_MASK);

    for (int i=0; i<36; i++)
    {
        // one bit set i positions from the end. set the ones_mask to 1 in the corresponding bit
        if (mask_str[35-i] == '1')
        {
            set_ones_mask += (ONE_BIT << i);
        }
        if (mask_str[35-i] == 'X')
        {
            clear_x_mask -= (ONE_BIT << i);
            int prior_size = x_masks.size();
            for (int j=0; j<prior_size; j++)
            {
                uint64_t new_x_mask = x_masks[j];
                new_x_mask += (ONE_BIT << i);
                x_masks.push_back(new_x_mask);
            }
        }
    }
    
#ifdef DEBUG_DAY_14
    cout << "Mask " << mask_str << " results in " << endl << " set_ones_mask " << std::bitset<32>(set_ones_mask) << endl << " clear_x_mask  " << std::bitset<32>(clear_x_mask) << endl;
    cout << " There are " << x_masks.size() << " x masks: " << endl;
    for (int i=0; i<x_masks.size(); i++)
    {
        cout << "  " << std::bitset<32>(x_masks[i]) << endl;
    }
#endif
    return;
}

/*
There is a multistep process to get all of the addresses from the base address and the mask.

* Perform `base_address` OR `set_ones_mask` to set all of the ones bits and store in `base_address`
* Perform `base_address` AND `clear_x_mask` to set all of the bits for the X's to 0. Store in `base_address`.
* Loop over `x_masks` list with `x_mask`
    * Append `base_address` OR `x_mask` and add that to the addresses list.
* Return the addresses list.
*/

vector<uint64_t> AocDay14::generate_address_list(uint64_t base_address, uint64_t set_ones_mask, uint64_t clear_x_mask, vector<uint64_t> x_masks)
{
    vector<uint64_t> addresses;
#ifdef DEBUG_DAY_14
    cout << "Generating address list values for base address " << base_address << " " << std::bitset<32>(base_address) << endl;
#endif
    base_address = base_address | set_ones_mask;
#ifdef DEBUG_DAY_14
    cout << " After applying ones, the base address is " << base_address << " " << std::bitset<32>(base_address) << endl;
#endif
    base_address = base_address & clear_x_mask;
#ifdef DEBUG_DAY_14
    cout << " After clearing x values, the base address is " << base_address << " " << std::bitset<32>(base_address) << endl;
#endif
    for (int i=0; i<x_masks.size(); i++)
    {
        uint64_t address = base_address | x_masks[i];
#ifdef DEBUG_DAY_14
        cout << " The destination address with x_mask " << std::bitset<32>(x_masks[i]) << " is " << address << " " << std::bitset<32>(address) << endl;
#endif
        addresses.push_back(address);
    }
    return addresses;
}

/*
* Parse the input into a list of strings.
* Create an empty map for memory.
* Loop over each line
    * If the line matches the mask regex:
        * Extract the mask from the capture group.
        * Create the set_ones_mask, clear_x_mask, and x_masks list from this mask.
    * If the line matches the mem regex:
        * Extract the address and value from the capture groups.
        * Generate the list of addresses from the base_address and current set of masks.
        * Store the value for each of the addresses in the list.
    * Else
        * Throw an error - did something wrong :-)
* Loop over the map, summing the values and return that sum.
*/

string AocDay14::part2(string filename, vector<string> extra_args)
{
    vector<string> lines = read_input(filename);
    map<uint64_t, uint64_t> memory;
    string mask;
    uint64_t set_ones_mask, clear_x_mask;
    vector<uint64_t> x_masks;
    
    for (vector<string>::iterator iter = lines.begin(); iter != lines.end(); ++iter)
    {
        string line = *iter;
        
        match_results<std::string::const_iterator> matches;
    
        // First search to see if it is a mask line to see if this matches a line with no contents
        regex_search(line, matches, mask_regex);
        if (matches.size() > 0)
        {
            mask = matches[1].str();
            setup_address_mask_values(mask, set_ones_mask, clear_x_mask, x_masks);
        }
        else
        {
            // Not a mask line. Search to see if it is a mem line
            regex_search(line, matches, mem_regex);
            if (matches.size() > 0)
            {
                uint64_t address = strtoull(matches[1].str().c_str(), NULL, 10);
                uint64_t value = strtoull(matches[2].str().c_str(), NULL, 10);
            
                vector<uint64_t> addresses = generate_address_list(address, set_ones_mask, clear_x_mask, x_masks);
        
                for (int i=0; i<addresses.size(); i++)
                {
#ifdef DEBUG_DAY_14
                    cout << "Setting " << value << " in address " << addresses[i] << endl;
#endif
                    memory[addresses[i]] = value;
                }
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
