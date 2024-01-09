// 2023/12/22 14:55:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Value.h"
#include "Display/Text.h"
#include <cstdlib>
#include <cstring>


void Value::Draw(int x, int y)
{
    char string[32];

    char *pointer = string;

    int value = munits;

    if (value < 0)
    {
        value = -value;
        string[0] = '-';
        pointer++;
    }

    if (value < 1000)
    {
        itoa(value, pointer, 10);
        std::strcat(string, "m");
    }
    else
    {
        int int_value = value / 1000;

        itoa(int_value, pointer, 10);

        std::strcat(string, ",");

        value = value - int_value * 1000;

        itoa(value, string + std::strlen(string), 10);
    }

    std::strcat(string, (unit == Unit::Volts) ? "V" : "s");
    Text(string).Write(x, y);
}
