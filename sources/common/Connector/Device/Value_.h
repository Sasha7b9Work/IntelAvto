// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Parameter;
struct Value;


struct TypeValue
{
    enum E
    {
        Voltage,    // Вольты
        Time,       // Миллисекунды
        Raw         // Количество, без единиц измерения
    };
};


struct DrawStruct
{
    void PressKey(int);
    void Draw(int x, int y) const;
    void Set(Parameter *_param);
    bool ToValue(Value *) const;
private:
    bool ConsistDot() const;
    void SetSymbolToCurrentPos(char);
    int NumSymbols() const;

    void IncreaseInPosition(int);
    void DecreaseInPosition(int);

    Parameter *parameter;
    int index;                          // Текущий разряд. 0 - "-", если есть
    static const int SIZE_BUFER = 10;   // Максимальное количество хранимх разрядов вместе с завершающим нулём
    char symbols[SIZE_BUFER];
};


struct Value
{
    Value(int value, TypeValue::E type)
    {
        raw = value > 0 ? (uint)value : (uint)(-value);

        if (value < 0)
        {
            raw |= (uint)(1 << 31);
        }

        if (type == TypeValue::Raw)             // 29-й и 30-й биты чистые
        {

        }
        else if (type == TypeValue::Time)       // 30-й бит установлен
        {
            raw |= (1 << 30);
        }
        else if (type == TypeValue::Voltage)    // 29-й бит установлен
        {
            raw |= (1 << 29);
        }
    }

    Value (uint v) : raw(v) { }

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
        if (raw & (1 << 30))
        {
            return TypeValue::Time;
        }

        if (raw & (1 << 29))
        {
            return TypeValue::Voltage;
        }

        return TypeValue::Raw;
    }

    uint GetRaw() const { return raw; }

    // Вольты для напряжения, миллисекунды для времени
    int ToInt() const
    {
        int value = (int)(raw & 0x1FFFFFFF);        // Старший бит - знак, два следующие - тип

        return (raw & (uint)(1 << 31)) ? -value : value;
    }

    // В секунды и в вольты
    float ToFloat() const
    {
        float value = (float)ToInt();

        if (GetType() == TypeValue::Time)
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
    Voltage(int voltage) : Value(voltage, TypeValue::Voltage) { }
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
