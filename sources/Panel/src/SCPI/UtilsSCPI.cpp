// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Stack.h"
#include "SCPI/UtilsSCPI.h"


bool SCPI::SU::IsLineEnding(pchar *buffer)
{
    bool result;
    if((**buffer == 0x0D) || (**buffer == 0x0A))
    {
        result = true;
    }
    else
    {
        result = false;
    }

    if (result)
    {
        *buffer = (*buffer) + 1;
    }

    return result;
}


pchar SCPI::SU::BeginWith(pchar buffer, pchar word)
{
    while (*word)
    {
        if (*buffer == '\0')
        {
            return nullptr;
        }

        if (*word == *buffer)
        {
            ++word;
            ++buffer;
        }
        else
        {
            break;
        }
    }

    return (*word == '\0') ? buffer : nullptr;
}


bool SCPI::SU::ReadIntegerValue(pchar buffer, int *outValue, pchar *outEnd)
{
    int sign = 1;

    if (*buffer == '-')
    {
        sign = -1;
        buffer++;
    }

    ConverterToInteger converter;

    while (*buffer >= '0' && *buffer <= '9')
    {
        converter.Push(*buffer);
        buffer++;
    }

    *outValue = converter.IsInterger() ? converter.GetValue() : 0;

    *outValue = *outValue * sign;

    *outEnd = buffer;

    return converter.IsInterger();
}
