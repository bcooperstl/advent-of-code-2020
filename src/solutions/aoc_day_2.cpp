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
// min-max char: password
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
    m_min_occurrences = strtol(matches[1].str().c_str(), NULL, 10);
    m_max_occurrences = strtol(matches[2].str().c_str(), NULL, 10);
    m_letter = matches[3].str()[0];
    m_password = matches[4].str();
}

PasswordFileLine::~PasswordFileLine()
{
}

bool PasswordFileLine::isValid()
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
        if ((*line).isValid())
        {
            counter++;
        }
    }
    ostringstream out;
    out << counter;
    return out.str();
}

