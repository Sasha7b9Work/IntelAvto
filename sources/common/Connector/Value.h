// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Keyboard/Keyboard.h"


class Parameter;


struct Order
{
    enum E
    {
        Volts,
        MS,
        US
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


struct UValue
{
    union
    {
        uint64 raw;
        struct
        {
            uint units : 32;
            uint sign : 1;
            uint orders_units : 2;  // Разрядность юнитов : 0 - единицы (вольты), 1 - милли (секунды), 2 - микро (секунды)
        };
    };

    float ToFloat() const
    {
        float s = sign == 0 ? 1.0f : -1.0f;

        if (orders_units == Order::Volts)
        {
            return units * s;
        }
        else if (orders_units == Order::MS)
        {
            return units * 1e3f * s;
        }
        else if (orders_units == Order::US)
        {
            return units * 1e6f * s;
        }
    }
};


struct Value
{
    Value(uint units, Order::E order = Order::MS, int sign = 1)
    {
        raw.units = units;
        raw.sign = sign ? 1U : 0U;
        raw.orders_units = (uint)order;
    }

    void Draw(const Parameter *, int x, int y) const;

    static DrawStruct ds;

    void FromDrawStrut(const Value &min, const Value &max);

    float ToFloat() const { return raw.ToFloat(); }

    UValue GetRaw() const { return raw; }

private:

    UValue raw;
};


struct Voltage : public Value
{
    Voltage(uint units, int sign = 1) : Value(units, Order::Volts, sign) {}
};


struct Time : public Value
{
    Time(uint units) : Value(_munits, Unit::Seconds) {}
};
