#ifndef __AOC_DAY_4__
#define __AOC_DAY_4__

#include "aoc_day.h"

#include <map>

using namespace std;

#define BIRTH_YEAR "byr"
#define ISSUE_YEAR "iyr"
#define EXPIRATION_YEAR "eyr"
#define HEIGHT "hgt"
#define HAIR_COLOR "hcl"
#define EYE_COLOR "ecl"
#define PASSPORT_ID "pid"
#define COUNTRY_ID "cid"

class AttributeValidator
{
    protected:
        string m_key;
    public:
        AttributeValidator(string key);
        virtual bool isValidData(string input) = 0;
        string getKey();
};

class YearValidator : public AttributeValidator
{
    private:
        int m_min_year;
        int m_max_year;
    public:
        YearValidator(string key, int min_year, int max_year);
        bool isValidData(string input);
};

class HeightValidator : public AttributeValidator
{
    public:
        HeightValidator();
        bool isValidData(string input);
};

class HairColorValidator : public AttributeValidator
{
    public:
        HairColorValidator();
        bool isValidData(string input);
};

class EyeColorValidator : public AttributeValidator
{
private:
        static vector<string> allowed_eye_colors;
public:
        EyeColorValidator();
        bool isValidData(string input);
};

class PassportIdValidator : public AttributeValidator
{
    public:
        PassportIdValidator();
        bool isValidData(string input);
};

class PassportData
{
    private:
        static vector<string> required_attributes;
        map<string, string> m_attributes;
    public:
        PassportData();
        ~PassportData();
        void setAttribute(string attribute, string value);
        bool isValidAttributes();
        bool isValidDataValues(vector<AttributeValidator *> validators);
};

class AocDay4 : public AocDay
{
    private:
        vector<PassportData *> parse_input(string filename);
    public:
        AocDay4();
        ~AocDay4();
        string part1(string filename, vector<string> extra_args);
        string part2(string filename, vector<string> extra_args);
};


#endif
