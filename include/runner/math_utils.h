#ifndef __MATH_UTILS_H__
#define __MATH_UTILS_H__

#include <cstdint>

using namespace std;

class MathUtils {
    public:
        static int64_t modulo_inverse(int64_t value, int64_t modulus);
};

#endif
