// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Keyboard.h"


class Parameter;


struct Unit
{
    enum E
    {
        Seconds,
        Volts,
        Raw,
        Count
    };
};


struct DrawStruct
{
    void PressKey(Key::E);
    void Draw(int x, int y) const;
    void Clear(Parameter *_param) { index = 0; parameter = _param; }
    bool ToMicroUnits(int *result) const;
private:
    bool ConsistDot() const;
    void AppendSymbol(char);
    Parameter *parameter;
    int index;
    static const int SIZE_BUFER = 8;
    char symbols[SIZE_BUFER];
};


struct Value
{
    Value(int _munits = 0, Unit::E u = Unit::Raw) : munits(_munits), unit(u)  { }

    void Draw(const Parameter *, int x, int y) const;

    static DrawStruct ds;

    void FromDrawStrut(const Value &min, const Value &max);

    float ToFloat() const;

    int GetRaw() const { return munits; }

private:

    int munits;     // Значение в миллиюнитах (например, 1 = 1 мс)

    Unit::E unit;
};


struct Voltage : public Value
{
    Voltage(int _munits) : Value(_munits, Unit::Volts) {}
};


struct Time : public Value
{
    Time(int _munits) : Value(_munits, Unit::Seconds) {}
};
