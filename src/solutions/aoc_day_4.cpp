#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cctype>

#include "aoc_day_4.h"
#include "file_utils.h"

using namespace std;

vector<string> PassportData::required_attributes {BIRTH_YEAR, ISSUE_YEAR, EXPIRATION_YEAR, HEIGHT, HAIR_COLOR, EYE_COLOR, PASSPORT_ID};

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

bool PassportData::isValidAttributes()
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

bool PassportData::isValidDataValues(vector<AttributeValidator *> validators)
{
    cout << "Deep validation for new record " << endl;
    for (vector<AttributeValidator *>::iterator validator_iter=validators.begin(); validator_iter != validators.end(); ++validator_iter)
    {
        AttributeValidator * validator = *validator_iter;
        string key = validator->getKey();
        string value = m_attributes[key];
        cout << "Validating " << key << " for value [" << value << "]" << endl;
        if (!validator->isValidData(value))
        {
            cout << "    INVALID" << endl;
            return false;
        }
    }
    return true;
}

AttributeValidator::AttributeValidator(string key)
{
    m_key = key;
}

string AttributeValidator::getKey()
{
    return m_key;
}

YearValidator::YearValidator(string key, int min_year, int max_year):AttributeValidator(key)
{
    m_min_year = min_year;
    m_max_year = max_year;
}

// four digits; at least m_min_year and at most m_max_year
// can be used for birth year, issue year, expiration year
bool YearValidator::isValidData(string input)
{
    if (input.length() != 4)
    {
        return false;
    }
    long year = strtol(input.c_str(), NULL, 10);
    return (year >= m_min_year && year <= m_max_year);
}

HeightValidator::HeightValidator():AttributeValidator(HEIGHT)
{
}

// Height:
// a number followed by either cm or in:
// If cm, the number must be at least 150 and at most 193.
// If in, the number must be at least 59 and at most 76.

bool HeightValidator::isValidData(string input)
{
    int height = 0;
    size_t in = input.find("in", 0);
    size_t cm = input.find("cm", 0);
    if (in != string::npos && cm == string::npos) // inches found
    {
        for (size_t i=0; i<in; i++)
        {
            // not a digit in the char precedign "in". invalid data
            if (!isdigit(input[i]))
            {
                return false;
            }
            height *= 10;
            height += (input[i] - '0');
        }
        return (height >= 59 && height <= 76);
    }
    else if (cm != string::npos && in == string::npos) // cm found
    {
        for (size_t i=0; i<cm; i++)
        {
            // not a digit in the char precedign "cm". invalid data
            if (!isdigit(input[i]))
            {
                return false;
            }
            height *= 10;
            height += (input[i] - '0');
        }
        return (height >= 150 && height <= 193);
    }
    return false;
}

HairColorValidator::HairColorValidator():AttributeValidator(HAIR_COLOR)
{
}

// Hair Color: a # followed by exactly six characters 0-9 or a-f
bool HairColorValidator::isValidData(string input)
{
    if (input.length() != 7)
    {
        return false;
    }
    if (input[0] != '#')
    {
        return false;
    }
    for (int i=1; i<= 6; i++)
    {
        // this is valid if it is a digit or if it is between a and f
        if (!( isdigit(input[i]) || (input[i] >= 'a' && input[i] <= 'f')))
        {
            return false;
        }
    }
    return true;
}


vector<string> EyeColorValidator::allowed_eye_colors {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

EyeColorValidator::EyeColorValidator():AttributeValidator(EYE_COLOR)
{
}

// Eye Color: exactly one of: amb blu brn gry grn hzl oth
bool EyeColorValidator::isValidData(string input)
{
    for (vector<string>::iterator iter = allowed_eye_colors.begin(); iter != allowed_eye_colors.end(); ++iter)
    {
        if (input == *iter)
        {
            return true;
        }
    }
    return false;
}

PassportIdValidator::PassportIdValidator():AttributeValidator(PASSPORT_ID)
{
}

// PassportID a nine-digit number, including leading zeroes
bool PassportIdValidator::isValidData(string input)
{
    if (input.length() != 9)
    {
        return false;
    }
    for (int i=0; i< 9; i++)
    {
        if (!isdigit(input[i]))
        {
            return false;
        }
    }
    return true;
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
        if ((*data_iter)->isValidAttributes())
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

string AocDay4::part2(string filename, vector<string> extra_args)
{
    vector<PassportData *> data = parse_input(filename);
    
    YearValidator birthYearValidator(BIRTH_YEAR, 1920, 2002);
    YearValidator issueYearValidator(ISSUE_YEAR, 2010, 2020);
    YearValidator expirationYearValidator(EXPIRATION_YEAR, 2020, 2030);
    HeightValidator heightValidator;
    HairColorValidator hairColorValidator;
    EyeColorValidator eyeColorValidator;
    PassportIdValidator passportIdValidator;
    
    vector<AttributeValidator * > validators;
    validators.push_back(&birthYearValidator);
    validators.push_back(&issueYearValidator);
    validators.push_back(&expirationYearValidator);
    validators.push_back(&heightValidator);
    validators.push_back(&hairColorValidator);
    validators.push_back(&eyeColorValidator);
    validators.push_back(&passportIdValidator);
    
    
    long counter = 0;
    for (vector<PassportData *>::iterator data_iter = data.begin(); data_iter != data.end(); ++data_iter)
    {
        if ((*data_iter)->isValidAttributes())
        {
            if ((*data_iter)->isValidDataValues(validators))
            {
                counter++;
            }
        }
        
        // Nothing else to do here; might as well do my dynamic memory cleanup instead of looping again
        delete *data_iter;
    }
    
    ostringstream out;
    out << counter;
    return out.str();
}
