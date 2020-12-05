#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "aoc_day_4.h"
#include "file_utils.h"

using namespace std;

vector<string> PassportData::required_attributes {"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};

PassportData::PassportData()
{
}

PassportData::~PassportData()
{
    m_attributes.clear();
}

void PassportData::setAttribute(string attribute, string value)
{
    m_attributes[attribute]=value;
}

bool PassportData::isValid()
{
    for (vector<string>::iterator attr_iter=required_attributes.begin(); attr_iter != required_attributes.end(); ++attr_iter)
    {
        if (m_attributes.find(*attr_iter) == m_attributes.end())
        {
            return false;
        }
    }
    return true;
}

// four digits; at least 1920 and at most 2002
bool BirthYearValidator::isValidData(string input)
{
    if (input.length() != 4)
    {
        return false;
    }
    long year = strtol(input.c_str(), NULL, 10);
    return (year >= 1920 && year <= 2002);
}


AocDay4::AocDay4():AocDay(4)
{
}

AocDay4::~AocDay4()
{
}

/* Design
   * Create a new PassportData
   * Loop over the lines of input
       * If a blank line is detected
           * Append the current PassportData object to the list
           * Create a new object for the next entry
       * Else (not a blank line)
           * Use my existing function to split based on the spaces. This will give me 1 or more key:value pairs per line.
           * Loop over the key:value pairs
               * Split the string into the key and value.
               * Call the setAttribute method on the PassportData object
   * Append the last PassportData object to the list.
*/
vector<PassportData *> AocDay4::parse_input(string filename)
{
    FileUtils fileutils;
    vector<string> raw_lines;
    vector<PassportData *> data;
    if (!fileutils.read_as_list_of_strings(filename, raw_lines))
    {
        cerr << "Error reading in the data from " << filename << endl;
        return data;
    }

    PassportData * currentDatum = new PassportData();
    cout << "Starting with first PassportData" << endl;
    for (vector<string>::iterator iter = raw_lines.begin(); iter != raw_lines.end(); ++iter)
    {
        string line = *iter;
        if (line.length() == 0)
        {
            data.push_back(currentDatum);
            cout << "Appending PassportData and starting new PassportData" << endl;
            currentDatum = new PassportData();
        }
        else
        {
            size_t current = 0;
            size_t colon = line.find(":", current);
            size_t space = line.find(" ", colon);
            while (space != string::npos)
            {
                string key = line.substr(current, colon-current);
                string value = line.substr(colon + 1, space - (colon + 1));
                cout << "key: [" << key << "] value: [" << value << "]" << endl;
                currentDatum->setAttribute(key, value);
                current = space + 1; // need +1 to skip the space
                colon = line.find(":", current);
                space = line.find(" ", colon);
            }
            string key = line.substr(current, colon-current);
            string value = line.substr(colon + 1, space - (colon + 1));
            cout << "key: [" << key << "] value: [" << value << "]" << endl;
            currentDatum->setAttribute(key, value);
        }
    }
    cout << "Appending final PassportData" << endl;
    data.push_back(currentDatum);
    return data;
}

string AocDay4::part1(string filename, vector<string> extra_args)
{
    vector<PassportData *> data = parse_input(filename);
    long counter = 0;
    for (vector<PassportData *>::iterator data_iter = data.begin(); data_iter != data.end(); ++data_iter)
    {
        if ((*data_iter)->isValid())
        {
            counter++;
        }
        // Nothing else to do here; might as well do my dynamic memory cleanup instead of looping again
        delete *data_iter;
    }
    
    ostringstream out;
    out << counter;
    return out.str();
}
