// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"
#include <limits>


class Float
{
public:

    Float(float v);

    String ToString(bool alwaysSign, int numDigits) const;

    static const float ERROR;

private:

    float value;
};
