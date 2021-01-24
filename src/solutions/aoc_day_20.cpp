#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <iterator>

#include "aoc_day_20.h"
#include "file_utils.h"

using namespace std;

AocDay20::AocDay20():AocDay(20)
{
}

AocDay20::~AocDay20()
{
}

void AocDay20::parse_input(string filename, vector<Tile *> & tiles)
{
    FileUtils fileutils;
    vector<string> data;
    if (!fileutils.read_as_list_of_strings(filename, data))
    {
        cerr << "Error reading in the data from " << filename << endl;
    }
    return;
}


string AocDay20::part1(string filename, vector<string> extra_args)
{
    vector<Tile *> tiles;
    parse_input(filename, tiles);
    long long product = 1;

    ostringstream out;
    out << product;
    return out.str();
}

