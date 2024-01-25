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
        Null,
        Count
    };
};


struct DrawStruct
{
    DrawStruct()     { Clear();      }
    void Clear()     { position = 0; }
    void PressKey(Key::E);
    void Draw() const;
    int position;
    static const int SIZE_BUFER = 16;
    char symbols[SIZE_BUFER];
};


struct Value
{
    Value(int _munits = 0, Unit::E u = Unit::Null) : munits(_munits), unit(u)  { }

    void Draw(const Parameter *, int x, int y) const;

    void FromDataStruct();

    static DrawStruct ds;

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
