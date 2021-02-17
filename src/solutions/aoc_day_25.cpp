#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <cstdint>

#include "aoc_day_25.h"
#include "file_utils.h"

using namespace std;

AocDay25::AocDay25():AocDay(25)
{
}

AocDay25::~AocDay25()
{
}

vector<long> AocDay25::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    vector<long> data;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return data;
    }
    for (vector<string>::iterator iter = raw_lines.begin(); iter != raw_lines.end(); ++iter)
    {
        long l;
        string to_convert = *iter;
        l = strtol(to_convert.c_str(), NULL, 10);
        data.push_back(l);
    }
    return data;
}

string AocDay25::part1(string filename, vector<string> extra_args)
{
    cout << "IN DAY 25" << endl;
    vector<long> entries = read_input(filename);
    uint64_t card_public_key = entries[0];
    uint64_t door_public_key = entries[1];
    
    uint64_t card_loop_count = 0;
    uint64_t door_loop_count = 0;
    
    uint64_t loop_count = 0;
    
    uint64_t working_value = 1;
    
    uint64_t subject_number = 7;
    
    uint64_t modulo = 20201227;
    
    cout << "Card public key = " << card_public_key << endl;
    cout << "Door public key = " << door_public_key << endl;
    
    while (card_loop_count == 0 || door_loop_count == 0)
    {
        
        loop_count++;
        working_value = (working_value * subject_number) % modulo;
        if (working_value == card_public_key)
        {
            cout << "Card loop count found to be " << loop_count << endl;
            card_loop_count = loop_count;
        }
        if (working_value == door_public_key)
        {
            cout << "Door loop count found to be " << loop_count << endl;
            door_loop_count = loop_count;
        }
    }
    
    uint64_t key_by_card_pk_door_lc = 1;
    uint64_t key_by_door_pk_card_lc = 1;
    
    for (uint64_t i=0; i<door_loop_count; i++)
    {
        key_by_card_pk_door_lc = (key_by_card_pk_door_lc * card_public_key) % modulo;
    }
    
    for (uint64_t i=0; i<card_loop_count; i++)
    {
        key_by_door_pk_card_lc = (key_by_door_pk_card_lc * door_public_key) % modulo;
    }
    
    if (key_by_card_pk_door_lc != key_by_door_pk_card_lc)
    {
        cout << "MISMATCH VALUES!! key_by_card_pk_door_lc=" << key_by_card_pk_door_lc << " key_by_door_pk_card_lc=" << key_by_door_pk_card_lc << endl;
    }
    
    ostringstream out;
    out << key_by_card_pk_door_lc;
    return out.str();
}

