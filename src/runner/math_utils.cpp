#include <iostream>
#include <cstdint>

#include "math_utils.h"

#define DEBUG_MATH

#define MAX_EUCLID_STEPS 64

using namespace std;

int64_t MathUtils::modulo_inverse(int64_t value, int64_t modulus)
{
    int64_t a[MAX_EUCLID_STEPS+1];
    int64_t b[MAX_EUCLID_STEPS+1];
    int64_t c[MAX_EUCLID_STEPS+1];
    int64_t d[MAX_EUCLID_STEPS+1];
    
#ifdef DEBUG_MATH
    cout << "Calculating modulo inverse for " << value << " * x = 1 modulo " << modulus << endl;
#endif

    a[0] = value;
    b[0] = modulus;
    int i=0;
    
    while (i < MAX_EUCLID_STEPS)
    {
        c[i]=a[i]/b[i];
        d[i]=a[i]%b[i];
#ifdef DEBUG_MATH
        cout << "Downward Equation " << i << ": " << a[i] << " = " << b[i] << "*" << c[i] << " + " << d[i] << endl;
#endif
        a[i+1]=b[i];
        b[i+1]=d[i];
        if (d[i] == 1)
        {
#ifdef DEBUG_MATH
            cout << " Done working down to 1" << endl;
#endif
            break;
        }
        i++;
    }
    
    if (i == MAX_EUCLID_STEPS)
    {
        cerr << "*****MODULE_INVERSE overflow. Increase number of steps*****" << endl;
        return 0;
    }
    
    return 0;
}
