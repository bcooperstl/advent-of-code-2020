#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

#include "aoc_day_1.h"
#include "file_utils.h"

using namespace std;

AocDay1::AocDay1():AocDay(1)
{
}

AocDay1::~AocDay1()
{
}

vector<long> AocDay1::read_input(string filename)
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

string AocDay1::part1(string filename, vector<string> extra_args)
{
    vector<long> entries = read_input(filename);
    long product = 0;
    for (vector<long>::iterator i = entries.begin(); i != entries.end(); ++i)
    {
        for (vector<long>::iterator j = next(i); j != entries.end(); ++j)
        {
            if ((*i)+(*j) == 2020)
            {
                product = (*i) * (*j);
                break;
            }
        }
        if (product != 0)
        {
            break;
        }
    }
    ostringstream out;
    out << product;
    return out.str();
}

string AocDay1::part2(string filename, vector<string> extra_args)
{
    vector<long> entries = read_input(filename);
    long product = 0;
    for (vector<long>::iterator i = entries.begin(); i != entries.end(); ++i)
    {
        for (vector<long>::iterator j = next(i); j != entries.end(); ++j)
        {
            for (vector<long>::iterator k = next(j); k != entries.end(); ++k)
            {
                if ((*i)+(*j)+(*k) == 2020)
                {
                    product = (*i) * (*j) * (*k);
                    break;
                }
            }
        }
        if (product != 0)
        {
            break;
        }
    }
    ostringstream out;
    out << product;
    return out.str();
}
