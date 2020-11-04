#include <iostream>
#include <vector>

#include "aoc_tests.h"
#include "aoc_test.h"
#include "constants.h"
#include "file_utils.h"

AocTests::AocTests()
{
    load_tests();
}

AocTests::~AocTests()
{
}

void AocTests::load_tests()
{
    std::vector<std::vector<std::string>> test_index_contents;
    FileUtils fileutils;
    std::string index_file = std::string(DATA_DIR)+std::string(TEST_INDEX_FILE);
    if (!fileutils.read_as_list_of_split_strings(index_file, test_index_contents, TEST_INDEX_DELIM, TEST_INDEX_QUOTE, TEST_INDEX_COMMENT))
    {
        std::cerr << "Error reading test index file " << index_file << std::endl;
        return;
    }
    
    for (std::vector<std::vector<std::string>>::iterator test_iter = test_index_contents.begin(); test_iter != test_index_contents.end(); ++test_iter)
    {
        std::vector<std::string> test_parameters = *test_iter;
        std::vector<std::string>::iterator parm_iter=test_parameters.begin();
        // format is day,part,filename,expected[,...extra args]
        // TODO: Add debugging library
        long day, part;
        std::string filename, expected;
        std::vector<std::string> extra_args;
        if (!fileutils.safe_strtol(*parm_iter, day))
        {
            std::cerr << "Error parsing day from " << *parm_iter << std::endl;
            return;
        }
        ++parm_iter;
        if (!fileutils.safe_strtol(*parm_iter, part))
        {
            std::cerr << "Error parsing part from " << *parm_iter << std::endl;
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

std::vector<AocTest> AocTests::get_all_tests()
{
    return m_tests;
}

std::vector<AocTest> AocTests::filter_tests(int day, int part)
{
    std::vector<AocTest> matching_tests;
    
    for (std::vector<AocTest>::iterator test_iter = m_tests.begin(); test_iter !=m_tests.end(); ++test_iter)
    {
        if ((*test_iter).matches(day, part))
        {
            matching_tests.push_back(*test_iter);
        }
    }
    
    return matching_tests;
}
