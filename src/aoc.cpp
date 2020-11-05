#include <string>
#include <sstream>
#include <iostream>

#include "aoc_days.h"
#include "aoc_tests.h"


void usage(string prog_name)
{
    cerr << "Usage for " << prog_name << endl;
    cerr << "Run one file: " << prog_name << " -d day -p part -f filename [extra_args...]" << endl;
    cerr << "Run one day/part tests: " << prog_name << " -d day -p part -t" << endl;
    cerr << "Run full regression test: " << prog_name << " -R" << endl;
    // TODO: Add debugging flags, options etc
}

int main (int argc, char * argv[])
{
    AocDays days;
    AocTests tests;
    int day = 0;
    int part = 0;
    bool regression = false;
    string filename = "";
    string result = "";
    vector<string> extra_args;
    ostringstream test_summary;
    
    if (filename != "")
    {
        AocDay * aoc_day = days.get_day(day);
        if (!aoc_day)
        {
            cerr << "Program for day " << day << " not found!!!" << endl;
            exit(8);
        }
        if (part == 1)
        {
            result = aoc_day->part1(filename, extra_args);
        }
        else if (part == 2)
        {
            result = aoc_day->part2(filename, extra_args);
        }
        else
        {
            cerr << "Invalid part" << part << " specified!!!" << endl;
            exit(8);
        }
        cout << "***Day " << day << " Part " << part << " for file " << filename << " has result " << result << endl;
    }
    else // no filename; assume some type of testing mode
    {
        vector<AocTest> tests_to_run;
        if (regression)
        {
            cout << "Running full regression test for all days and parts!" << endl;
            tests_to_run = tests.get_all_tests();
        }
        else
        {
            cout << "Running tests for day " << day << " part " << part << endl;
            tests_to_run = tests.filter_tests(day, part);
        }
        for (vector<AocTest>::iterator test_iter = tests_to_run.begin(); test_iter != tests_to_run.end(); ++test_iter)
        {
            AocTest test = *test_iter;
            AocDay * aoc_day = days.get_day(test.get_day());
            if (!aoc_day)
            {
                cerr << "Program for day " << test.get_day() << " not found!!!" << endl;
                exit(8);
            }
            if (test.get_part() == 1)
            {
                result = aoc_day->part1(test.get_filename(), test.get_extra_args());
            }
            else if (test.get_part() == 2)
            {
                result = aoc_day->part2(test.get_filename(), test.get_extra_args());
            }
            else
            {
                cerr << "Invalid part" << test.get_part() << " specified!!!" << endl;
                exit(8);
            }
            if (result == test.get_expected_result())
            {
                test_summary << "++Day " << test.get_day() << " Part " << test.get_part() << "-" << test.get_filename() << " passed with result " << result << endl;
            }
            else
            {
                test_summary << "--Day " << test.get_day() << " Part " << test.get_part() << "-" << test.get_filename() << " FAILED expected=" << test.get_expected_result() << " actual=" << result << endl;
            }
        }
        cout << test_summary.str();
    }
    return 0;
}
