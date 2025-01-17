// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Keyboard/Keyboard.h"


class Parameter;


struct TypeValue
{
    enum E
    {
        Volts,      // Вольты
        MS,         // Миллисекунды
        RAW,        // Количество, без единиц измерения
        Count
    };
};


struct DrawStruct
{
    void PressKey(Key::E);
    void Draw(int x, int y) const;
    void Clear(Parameter *_param) { index = 0; parameter = _param; }
    bool ToRaw(uint *result, TypeValue::E) const;
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
    explicit Value(uint _raw) : raw(_raw) { }

    Value(int value, TypeValue::E type)
    {
        raw = (uint)value;

        if (value < 0)
        {
            raw |= (uint)(1 << 31);
        }

        if (type == TypeValue::MS)
        {
            raw |= (1 << 30);
        }
    }

    void Draw(const Parameter *, int x, int y) const;

    static DrawStruct ds;

    // Возвращает true, если значение находится в пределах [min, max]
    bool FromDrawStrut(const Value &min, const Value &max);

    TypeValue::E GetType() const
    {
        return ((raw & (1 << 30)) == 0) ? TypeValue::Volts : TypeValue::MS;
    }

    uint GetRaw() const { return raw; }

    // Вольты для напряжения, миллисекунды для времени
    int ToInt() const
    {
        int value = (int)(raw & 0x3FFFFFFF);

        if (raw & (uint)(1 << 31))
        {
            value = -value;
        }

        return value;
    }

    // В секунды и в вольты
    float ToFloat() const
    {
        float value = (float)ToInt();

        if (GetType() == TypeValue::MS)
        {
            value *= 1e-3f;
        }

        return value;
    }

private:

    // В бите 31 хранится знак : 0 - положительное, 1 - отрицательное
    // Бит 30 - тип : 0 - напряжение (В), 1 - время (мс)
    // Биты 0-29 - значение
    uint raw;
};


struct Voltage : public Value
{
    Voltage(int voltage) : Value(voltage, TypeValue::Volts) {}
};


struct Time : public Value
{
    Time(int ms) : Value(ms, TypeValue::MS) {}
};
