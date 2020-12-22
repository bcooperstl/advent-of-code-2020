#include <iostream>
#include <cstdint>
#include <cstdlib>

#include "math_utils.h"

using namespace std;

int main (int argc, char * argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " value modulus" << endl;
        exit(8);
    }
    int64_t value = strtoll(argv[1], NULL, 0);
    int64_t modulus = strtoll(argv[2], NULL, 0);
    int64_t result = MathUtils::modulo_inverse(value, modulus);
    
    cout << "Result is " << result << endl;
    return 0;
}
