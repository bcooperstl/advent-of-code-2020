#ifndef __AOC_DAY_4__
#define __AOC_DAY_4__

#include "aoc_day.h"

#include <map>

using namespace std;

class PassportData
{
    private:
        static vector<string> required_attributes;
        map<string, string> m_attributes;
    public:
        PassportData();
        ~PassportData();
        void setAttribute(string attribute, string value);
        bool isValid();
};

class AttributeValidator
{
    public:
        virtual bool isValidData(string input) = 0;
};

class BirthYearValidator : public AttributeValidator
{
    public:
        bool isValidData(string input);
};

class AocDay4 : public AocDay
{
    private:
        vector<PassportData *> parse_input(string filename);
    public:
        AocDay4();
        ~AocDay4();
        string part1(string filename, vector<string> extra_args);
        //string part2(string filename, vector<string> extra_args);
};


#endif
