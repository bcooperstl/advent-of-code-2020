#include <iostream>
#include <cstdint>

#include "math_utils.h"

//#define DEBUG_MODULO_INVERSE
//#define DEBUG_CRT

#define MAX_EUCLID_STEPS 64
#define MAX_CRT_EQUATIONS 64

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
    
#ifdef DEBUG_MODULO_INVERSE
    cout << "Calculating modulo inverse for " << value << " * x = 1 modulo " << modulus << endl;
#endif

    a[0] = value;
    b[0] = modulus;
    int i=0;
    
    while (i < MAX_EUCLID_STEPS)
    {
        c[i]=a[i]/b[i];
        d[i]=a[i]%b[i];
#ifdef DEBUG_MODULO_INVERSE
        cout << "Downward Equation " << i << ": " << a[i] << " = " << b[i] << "*" << c[i] << " + " << d[i] << endl;
#endif
        a[i+1]=b[i];
        b[i+1]=d[i];
        if (d[i] == 1)
        {
#ifdef DEBUG_MODULO_INVERSE
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
    
    if (i == 0)
    {
#ifdef DEBUG_MODULO_INVERSE
        cout << "Special case where modulo is foundon first equation. Returning 1 is the inverse" << endl;
#endif
        return 1;
    }

#ifdef DEBUG_MODULO_INVERSE
    cout << "Upward Equation " << i << ": 1 = (" << one_a_coef[i] << " * " << a[i] << ") + (" << one_b_coef[i] << " * " << b[i] << ")" << endl;
#endif

    for (int j=i-1; j>0; j--)
    {
        one_a_coef[j] = one_b_coef[j+1];
        one_b_coef[j] = one_a_coef[j+1]-(one_b_coef[j+1]*c[j]);
#ifdef DEBUG_MODULO_INVERSE
    cout << "Upward Equation " << j << ": 1 = (" << one_a_coef[j] << " * " << a[j] << ") + (" << one_b_coef[j] << " * " << b[j] << ")" << endl;
#endif
    }
    
    //one_b_coef[1] is our answer. need to mod is by the original modulus
    
    int64_t result = one_b_coef[1]%modulus;
    if (result < 0)
    {
        result = result+modulus;
    }
    
    if ((result * value) % modulus != 1)
    {
        cerr << "******INVALID MODULO INVERSE FOR " << value << " mod " << modulus << ". " << result << " DOES NOT WORK CORRECTLY" << endl;
    }        
    
    return result;
}

int64_t MathUtils::chinese_remainder_theorem(int num_equations, int64_t * values, int64_t * moduluses)
{
    int64_t mods_all = 1; // product of all moduluses
    int64_t mods_except[MAX_CRT_EQUATIONS]; // products of all moduluses except for the ith one_a_coef
    int64_t inverses[MAX_CRT_EQUATIONS]; // the module inverses
    int64_t result = 0;
    
    if (num_equations > MAX_CRT_EQUATIONS)
    {
        cerr << "*****CHINESE REMAINDER THEOREM overflow. Increase number of equations*****" << endl;
        return 0;
    }

    for (int i=0; i<num_equations; i++)
    {
#ifdef DEBUG_CRT
        cout << "Solving x = " << values[i] << " mod " << moduluses[i] << endl;
#endif
        mods_all *= moduluses[i];
        mods_except[i] = 1; // initialize this for the next step
    }
    
#ifdef DEBUG_CRT
    cout << "m (the multiplied modulus) is " << mods_all << endl;
#endif
    
    for (int i=0; i<num_equations; i++)
    {
        mods_except[i] = mods_all / moduluses[i];
#ifdef DEBUG_CRT
        cout << "M[" << i << "] = " << mods_except[i] << endl;
#endif
    }
    
    for (int i=0; i<num_equations; i++)
    {
        inverses[i]=modulo_inverse(mods_except[i], moduluses[i]);
#ifdef DEBUG_CRT
        cout << "Solved " << mods_except[i] << " * " << inverses[i] << " = 1 mod " << moduluses[i] << endl;
#endif
    }
    
    for (int i=0; i<num_equations; i++)
    {
#ifdef DEBUG_CRT
        cout << "Adding " << values[i] << "*" << mods_except[i] << "*" << inverses[i] << "=" << values[i]*mods_except[i]*inverses[i] << " to result" << endl;
#endif
        result += values[i]*mods_except[i]*inverses[i];
    }

#ifdef DEBUG_CRT
    cout << "Result is " << result << " before final modulo " << mods_all << endl;
#endif

    result = result % mods_all;
    
#ifdef DEBUG_CRT
    cout << "Final Chinese Remainder Theorem result is " << result << endl;
#endif
    
    for (int i=0; i<num_equations; i++)
    {
        if ((result % moduluses[i]) != values[i])
        {
            cerr << "******INVALID CHINESE REMAINDER THEORM APPLICATION FOR " << values[i] << " mod " << moduluses[i] << ". " << result << " DOES NOT WORK CORRECTLY" << endl;
        }        
    }
    
    return result;
}