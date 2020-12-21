#include <iostream>
#include <cstdint>
#include <cstdlib>

#include "math_utils.h"

using namespace std;

int main (int argc, char * argv[])
{
    if (argc != 1)
    {
        cerr << "Usage: " << argv[0] << endl;
        exit(8);
    }
    int64_t values[4] = {1,1,1,0};
    int64_t moduluses[4] = {3,4,5,7};
    
    int64_t result = MathUtils::chinese_remainder_theorem(4, values, moduluses);
    
    cout << "Result is " << result << endl;
    return 0;
}
