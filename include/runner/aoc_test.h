#ifndef __AOC_TEST_H__
#define __AOC_TEST_H__

#include <string>
#include <vector>

class AocTest
{
    protected:
        int m_day;
        int m_part;
        std::string m_filename;
        std::string m_expected_result;
        std::vector<std::string> m_extra_args;
    public:
        AocTest(int day, int part, std::string filename, std::string expected_result, std::vector<std::string> extra_args);
        ~AocTest();
        bool matches(int day, int part);
        bool compare_result(std::string actual_result);
};

#endif
