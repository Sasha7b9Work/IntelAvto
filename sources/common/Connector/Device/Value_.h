// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <cstdio>


struct TypeValue
{
    enum E
    {
        Voltage,    // Вольты
        Current,    // Амперы
        Time,       // Миллисекунды
        Raw         // Количество, без единиц измерения
    };
};


struct Value
{
    Value(int value, TypeValue::E type)
    {
        Set(value, type);
    }

    Value (uint v) : raw(v) { }

    void Set(int value, TypeValue::E type)
    {
        raw = value > 0 ? (uint)value : (uint)(-value);

        if (value < 0)
        {
            raw |= (uint)(1 << 31);
        }

        if (type == TypeValue::Raw)             // 29-й и 30-й биты чистые
        {

        }
        else if (type == TypeValue::Time)
        {
            raw |= (1 << 30);
            raw |= (1 << 29);
        }
        else if (type == TypeValue::Voltage)
        {
            raw |= (1 << 29);
        }
        else if (type == TypeValue::Current)
        {
            raw |= (1 << 30);
        }
    }

    void Draw(int x, int y) const;

    bool IsRaw() const
    {
        return GetType() == TypeValue::Raw;
    }

    bool IsVoltage() const
    {
        return GetType() == TypeValue::Voltage;
    }

    TypeValue::E GetType() const
    {
        if ((raw & (1 << 30)) && (raw & (1 << 29)))
        {
            return TypeValue::Time;
        }

        if (raw & (1 << 29))
        {
            return TypeValue::Voltage;
        }

        if (raw & (1 << 30))
        {
            return TypeValue::Current;
        }

        return TypeValue::Raw;
    }

    uint GetRaw() const { return raw; }

    // Милливольты для напряжения, миллисекунды для времени
    // MU - миллиюниты
    int ToMU() const
    {
        int value = (int)(raw & 0x1FFFFFFF);        // Старший бит - знак, два следующие - тип

        return (raw & (uint)(1 << 31)) ? -value : value;
    }

    // Целая часть
    int WholePart() const
    {
        return ToMU() / 1000;
    }

    // Дробная часть (количество миллиюнитов)
    int FractPart() const
    {
        return ToMU() - WholePart() * 1000;
    }

    char *FractPart(char buffer[4]) const
    {
        buffer[0] = '\0';

        int part = FractPart();

        if (part < 10)
        {
            std::sprintf(buffer, "00%d", part);
        }
        else if (part < 100)
        {
            std::sprintf(buffer, "0%d", part);
        }
        else
        {
            std::sprintf(buffer, "%d", part);
        }

        return buffer;
    }

    // В секунды и в вольты
    float ToUnits() const
    {
        return (float)ToMU() * 1e-3f;
    }

private:

    // В бите 31 хранится знак : 0 - положительное, 1 - отрицательное
    // Бит 30 - тип : 0 - напряжение (В), 1 - время (мс)
    // Биты 0-29 - значение
    uint raw;
};


struct Voltage : public Value
{
    explicit Voltage(int voltage) : Value(voltage, TypeValue::Voltage) { }

    void Set(int mV)
    {
        Value::Set(mV, TypeValue::Voltage);
    }
};


struct Current : public Value
{
    explicit Current(int current) : Value(current, TypeValue::Current) { }

    void Set(int mA)
    {
        Value::Set(mA, TypeValue::Current);
    }
};


struct Time : public Value
{
    Time(int ms) : Value(ms, TypeValue::Time) { }
};


struct Counter : public Value
{
    Counter(uint n) : Value((int)n, TypeValue::Raw) { }
};


struct ValueNull : public Counter
{
    ValueNull() : Counter(0) { }
};
