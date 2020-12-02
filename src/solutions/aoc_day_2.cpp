#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <regex>

#include "aoc_day_2.h"
#include "file_utils.h"

using namespace std;

// The regex format will have 4 subgroups
// Part 1: min-max char: password
// Part 2: pos1-pos2 char: password
regex PasswordFileLine::line_format("(\\d+)-(\\d+) ([a-zA-Z]): (.*)");

PasswordFileLine::PasswordFileLine(string line)
{
#ifdef DEBUG_REGEX
    cout << "Processing line [" << line << "]" << endl;
#endif
    match_results<std::string::const_iterator> matches;
    
    regex_search(line, matches, line_format);
    
#ifdef DEBUG_REGEX
    cout << "Match results for [" << line << "] has " << matches.size() << " matches" << endl;
    cout << "  1 " << matches[1] << endl;
    cout << "  2 " << matches[2] << endl;
    cout << "  3 " << matches[3] << endl;
    cout << "  4 " << matches[4] << endl;
#endif
    // Part 1 variables
    m_min_occurrences = strtol(matches[1].str().c_str(), NULL, 10);
    m_max_occurrences = strtol(matches[2].str().c_str(), NULL, 10);
    // Part 2 variables
    m_matching_positions[0] = strtol(matches[1].str().c_str(), NULL, 10);
    m_matching_positions[1] = strtol(matches[2].str().c_str(), NULL, 10);
    // Common variables
    m_letter = matches[3].str()[0];
    m_password = matches[4].str();
}

PasswordFileLine::~PasswordFileLine()
{
}

bool PasswordFileLine::isValidPart1()
{
    int counter = 0;
    for (int i=0; i<m_password.length(); i++)
    {
        if (m_password[i]==m_letter)
        {
            counter++;
        }
    }
    return ((m_min_occurrences <= counter) && (counter <= m_max_occurrences));
}

bool PasswordFileLine::isValidPart2()
{
    int matches = 0;
    // The password string is 1-indexed, so need to subtract 1 from the position to get the 0-indexed location
    if (m_password[m_matching_positions[0]-1] == m_letter)
    {
        matches++;
    }
    if (m_password[m_matching_positions[1]-1] == m_letter)
    {
        matches++;
    }
    
    return (matches == 1);
}

AocDay2::AocDay2():AocDay(2)
{
}

AocDay2::~AocDay2()
{
}

vector<PasswordFileLine> AocDay2::read_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    vector<PasswordFileLine> data;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return data;
    }
    for (vector<string>::iterator iter = raw_lines.begin(); iter != raw_lines.end(); ++iter)
    {
        data.push_back(PasswordFileLine(*iter));
    }
    return data;
}

string AocDay2::part1(string filename, vector<string> extra_args)
{
    vector<PasswordFileLine> data = read_input(filename);
    long counter = 0;
    for (vector<PasswordFileLine>::iterator line = data.begin(); line != data.end(); ++line)
    {
        if ((*line).isValidPart1())
        {
            counter++;
        }
    }
    ostringstream out;
    out << counter;
    return out.str();
}

string AocDay2::part2(string filename, vector<string> extra_args)
{
    vector<PasswordFileLine> data = read_input(filename);
    long counter = 0;
    for (vector<PasswordFileLine>::iterator line = data.begin(); line != data.end(); ++line)
    {
        if ((*line).isValidPart2())
        {
            counter++;
        }
    }
    ostringstream out;
    out << counter;
    return out.str();
}

