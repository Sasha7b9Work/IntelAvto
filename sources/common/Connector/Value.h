// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Keyboard/Keyboard.h"


class Parameter;


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

private:

    // В бите 31 хранится знак : 0 - положительное, 1 - отрицательное
    // Бит 30 - тип : 0 - напряжение (В), 1 - время (мс)
    // Биты 0-29 - значение
    uint raw;
};


struct TypeValue


struct Value
{
    Value(int value, Order::E order = Order::MS, int sign = 1)
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
