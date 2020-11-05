#include <iostream>
#include <vector>

#include "aoc_tests.h"
#include "aoc_test.h"
#include "constants.h"
#include "file_utils.h"

using namespace std;

AocTests::AocTests()
{
    load_tests();
}

AocTests::~AocTests()
{
}

void AocTests::load_tests()
{
    vector<vector<string>> test_index_contents;
    FileUtils fileutils;
    string index_file = string(DATA_DIR)+string(TEST_INDEX_FILE);
    if (!fileutils.read_as_list_of_split_strings(index_file, test_index_contents, TEST_INDEX_DELIM, TEST_INDEX_QUOTE, TEST_INDEX_COMMENT))
    {
        cerr << "Error reading test index file " << index_file << endl;
        return;
    }
    
    for (vector<vector<string>>::iterator test_iter = test_index_contents.begin(); test_iter != test_index_contents.end(); ++test_iter)
    {
        vector<string> test_parameters = *test_iter;
        vector<string>::iterator parm_iter=test_parameters.begin();
        // format is day,part,filename,expected[,...extra args]
        // TODO: Add debugging library
        long day, part;
        string filename, expected;
        vector<string> extra_args;
        if (!fileutils.safe_strtol(*parm_iter, day))
        {
            cerr << "Error parsing day from " << *parm_iter << endl;
            return;
        }
        ++parm_iter;
        if (!fileutils.safe_strtol(*parm_iter, part))
        {
            cerr << "Error parsing part from " << *parm_iter << endl;
            return;
        }
        ++parm_iter;
        filename = *parm_iter;
        ++parm_iter;
        expected = *parm_iter;
        ++parm_iter;
        while (parm_iter != test_parameters.end())
        {
            extra_args.push_back(*parm_iter);
            parm_iter++;
        }
        m_tests.push_back(AocTest(day, part, filename, expected, extra_args));
    }
}

vector<AocTest> AocTests::get_all_tests()
{
    return m_tests;
}

vector<AocTest> AocTests::filter_tests(int day, int part)
{
    vector<AocTest> matching_tests;
    
    for (vector<AocTest>::iterator test_iter = m_tests.begin(); test_iter !=m_tests.end(); ++test_iter)
    {
        if ((*test_iter).matches(day, part))
        {
            matching_tests.push_back(*test_iter);
        }
    }
    
    return matching_tests;
}