#ifndef __AOC_TESTS_H__
#define __AOC_TESTS_H__

#include <string>
#include <vector>

#include "aoc_test.h"

class AocTests
{
    private:
        std::vector<AocTest> m_tests;
        void load_tests();
    public:
        AocTests();
        ~AocTests();
        std::vector<AocTest> get_all_tests();
        std::vector<AocTest> filter_tests(int day, int part);
};

#endif
