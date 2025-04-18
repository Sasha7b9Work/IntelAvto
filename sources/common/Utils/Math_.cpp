// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Math_.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>


template void  Math::CircleIncrease<int8>(int8 *, int8, int8);
template void  Math::CircleIncrease<uint8>(uint8 *, uint8, uint8);
template void  Math::CircleIncrease<int16>(int16 *, int16, int16);
template void  Math::CircleIncrease<int>(int *, int, int);
template void  Math::CircleIncrease<uint>(uint *, uint, uint);

template void  Math::CircleDecrease<int8>(int8 *, int8, int8);
template void  Math::CircleDecrease<uint8>(uint8 *, uint8, uint8);
template void  Math::CircleDecrease<int16>(int16 *, int16, int16);
template void  Math::CircleDecrease<int>(int *, int, int);

template int16 Math::Abs<int16>(int16);
template int   Math::Abs<int>(int);
template int64 Math::Abs<int64>(int64);

int Math::DigitsInIntPart(float value)
{
    if (value == std::numeric_limits<float>::infinity())
    {
        return 2;
    }

    float absValue = std::fabsf(value);

    int num = 0;

    do
    {
        ++num;
        absValue /= 10.0F;
    } while (absValue >= 1.0F);

    return num;
}


float Math::RoundFloat(float value, int numDigits)
{
    if(value == std::numeric_limits<float>::infinity())
    {
        (void)value;
    }
    
    float absValue = std::fabsf(value);

    int digsInInt = Math::DigitsInIntPart(absValue);

    if (digsInInt < numDigits)  // �������������
    {
        int pow = Pow10(numDigits - digsInInt);
        absValue = (absValue * (float)pow + 0.5F) / static_cast<float>(pow);
    }

    return value > 0.0F ? absValue : -absValue;
}


bool Math::IsEquals(float x, float y)
{
    return std::fabsf(x - y) < std::numeric_limits<float>::epsilon();
}


template<class T>
T Math::Abs(T x)
{
     if (x < (T)0)
     {
         return -x; 
     }
     return x; 
}


template<class T>
void Math::CircleIncrease(T *value, T min, T max)
{
    if (*value < max)
    {
        ++(*value);
    }
    else
    {
        *value = (T)min;
    }
}


template<class T>
void Math::CircleDecrease(T *value, T min, T max)
{
    if (*value > min)
    {
        --(*value);
    }
    else
    {
        *value = (T)max;
    }
}


int Math::Pow10(int pow)
{
    int retValue = 1;

    while (pow--)
    {
        retValue *= 10;
    }

    return retValue;
}


int Math::Sign(int val)
{
    if (val < 0)
    {
        return -1;
    }

    return 1;
}


void Math::ItoA(int n, char _s[32])
{
    char *p = _s;

    if (n < 0)
    {
        n = -n;
        *p = '-';
        p++;
    }

    uint i = 0;
    uint j = 0;
    char c = '\0';

    do
    {
        p[i++] = (char)(n % 10 + '0');
    } while ((n /= 10) > 0);

    p[i] = '\0';

    // Reversing
    for (i = 0, j = std::strlen(p) - 1; i < j; i++, j--)
    {
        c = p[i];
        p[i] = p[j];
        p[j] = c;
    }
}
