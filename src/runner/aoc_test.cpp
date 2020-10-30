#include <string>
#include <vector>

#include "aoc_test.h"

AocTest::AocTest(int day, int part, std::string filename, std::string expected_result, std::vector<std::string> extra_args):m_day(day),m_part(part),m_filename(filename),m_expected_result(expected_result),m_extra_args(extra_args)
{
}

AocTest::~AocTest()
{
}

bool AocTest::matches(int day, int part)
{
    return (m_day == day) && (m_part == part);
}

bool AocTest::compare_result(std::string actual_result)
{
    return m_expected_result == actual_result;
}
