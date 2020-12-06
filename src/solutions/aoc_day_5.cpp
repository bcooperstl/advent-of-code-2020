#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "aoc_day_5.h"
#include "file_utils.h"

using namespace std;

#define FRONT_FROM 'F'
#define FRONT_TO '0'
#define BACK_FROM 'B'
#define BACK_TO '1'
#define LOWER_FROM 'L'
#define LOWER_TO '0'
#define UPPER_FROM 'R'
#define UPPER_TO '1'

AocDay5::AocDay5():AocDay(5)
{
}

AocDay5::~AocDay5()
{
}

int AocDay5::calculate_row(string boarding_pass)
{
    string row_id = boarding_pass.substr(0,7);
    replace(row_id.begin(), row_id.end(), FRONT_FROM, FRONT_TO);
    replace(row_id.begin(), row_id.end(), BACK_FROM, BACK_TO);
    return strtol(row_id.c_str(), NULL, 2);
}

int AocDay5::calculate_column(string boarding_pass)
{
    string col_id = boarding_pass.substr(7,3);
    replace(col_id.begin(), col_id.end(), LOWER_FROM, LOWER_TO);
    replace(col_id.begin(), col_id.end(), UPPER_FROM, UPPER_TO);
    return strtol(col_id.c_str(), NULL, 2);
}

int AocDay5::calculate_seat_id(int row, int column)
{
    return ((row * 8) + column);
}

vector<string> AocDay5::read_input(string filename)
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

string AocDay5::part1(string filename, vector<string> extra_args)
{
    vector<string> boarding_passes = read_input(filename);
    int max_seat_id = 0;
    
    for (vector<string>::iterator bp_iter = boarding_passes.begin(); bp_iter != boarding_passes.end(); ++bp_iter)
    {
        string bp = *bp_iter;
        int row = calculate_row(bp);
        int column = calculate_column(bp);
        int seat_id = calculate_seat_id(row, column);
        cout << "Boarding pass " << bp << " has row " << row << " column " << column << " seat ID " << seat_id << endl;
        if (seat_id > max_seat_id)
        {
            cout << "  New Maximum value " << seat_id << endl;
            max_seat_id = seat_id;
        }
    }
    ostringstream out;
    out << max_seat_id;
    return out.str();
}

string AocDay5::part2(string filename, vector<string> extra_args)
{
    vector<string> boarding_passes = read_input(filename);
    vector<long> seat_ids;
    
    for (vector<string>::iterator bp_iter = boarding_passes.begin(); bp_iter != boarding_passes.end(); ++bp_iter)
    {
        string bp = *bp_iter;
        int row = calculate_row(bp);
        int column = calculate_column(bp);
        int seat_id = calculate_seat_id(row, column);
        seat_ids.push_back(seat_id);
    }
    
    sort(seat_ids.begin(), seat_ids.end());
    
    int previous = seat_ids[0];
    int found = 0;
    for (int i=1; i<seat_ids.size(); i++)
    {
        if (seat_ids[i] != (previous + 1))
        {
            found = previous + 1;
            break;
        }
        previous = seat_ids[i];
    }
    
    ostringstream out;
    out << found;
    return out.str();
}
