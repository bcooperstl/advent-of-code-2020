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
    int64_t one_a_coef[MAX_EUCLID_STEPS+1];
    int64_t one_b_coef[MAX_EUCLID_STEPS+1];
    int64_t minus_1 = -1;
    
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
    
    one_a_coef[i]=1;
    one_b_coef[i]=minus_1*c[i];
    
#ifdef DEBUG_MATH
    cout << "Upward Equation " << i << ": 1 = (" << one_a_coef[i] << " * " << a[i] << ") + (" << one_b_coef[i] << " * " << b[i] << ")" << endl;
#endif

    for (int j=i-1; j>0; j--)
    {
        one_a_coef[j] = one_b_coef[j+1];
        one_b_coef[j] = one_a_coef[j+1]-(one_b_coef[j+1]*c[j]);
#ifdef DEBUG_MATH
    cout << "Upward Equation " << j << ": 1 = (" << one_a_coef[j] << " * " << a[j] << ") + (" << one_b_coef[j] << " * " << b[j] << ")" << endl;
#endif
    }
    
    //one_b_coef[1] is our answer. need to mod is by the original modulus
    
    int64_t result = one_b_coef[1]%modulus;
    if (result < 0)
    {
        result = result+modulus;
    }
    
    return result;
}
