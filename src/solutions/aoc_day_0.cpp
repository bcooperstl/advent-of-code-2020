#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "aoc_day_0.h"
#include "file_utils.h"

using namespace std;

AocDay0::AocDay0():AocDay(0)
{
}

AocDay0::~AocDay0()
{
}

vector<long> AocDay0::read_input(string filename)
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
        if ((*iter)[0]=='+')
        {
            if (!fileutils.safe_strtol((*iter).substr(1), l))
            {
                cerr << "Error converting " << (*iter).substr(1) << " to long" << endl;
                data.clear();
                return data;
            }
            data.push_back(l);
        }
        else
        {
            if (!fileutils.safe_strtol((*iter), l))
            {
                cerr << "Error converting " << (*iter) << " to long" << endl;
                data.clear();
                return data;
            }
            data.push_back(l);
        }
    }
    return data;
}

string AocDay0::part1(string filename, vector<string> extra_args)
{
    vector<long> data = read_input(filename);
    long sum = 0;
    for (vector<long>::iterator iter = data.begin(); iter != data.end(); ++iter)
    {
        sum+=*iter;
    }
    ostringstream out;
    out << sum;
    return out.str();
}

string AocDay0::part2(string filename, vector<string> extra_args)
{
    vector<long> data = read_input(filename);
    long sum = 0;
    for (vector<long>::iterator iter = data.begin(); iter != data.end(); ++iter)
    {
        sum-=*iter;
    }
    ostringstream out;
    out << sum;
    return out.str();
}
