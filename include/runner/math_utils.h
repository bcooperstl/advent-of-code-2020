#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__

#include <cstdint>

using namespace std;

class MathUtils {
    public:
        static int64_t modulo_inverse(int64_t value, int64_t modulus);
        static int64_t chinese_remainder_theorem(int num_equations, int64_t * values, int64_t * moduluses);
};

#endif
