// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Range
{
    Range(float _min = 0.0f, float _max = 0.0f) : min(_min), max(_max), value(0.0f) { }
    float min;
    float max;
    float value;
};


struct Value
{
    Value(Range _12V = Range(), Range _24V = Range()) : range12(_12V), range24(_24V) {}
    Range range12;
    Range range24;
};
