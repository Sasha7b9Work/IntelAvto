// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct TypeValue
{
    enum E
    {
        Voltage,    // ������
        Time,       // ������������
        Raw         // ����������, ��� ������ ���������
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

        if (type == TypeValue::Raw)             // 29-� � 30-� ���� ������
        {

        }
        else if (type == TypeValue::Time)       // 30-� ��� ����������
        {
            raw |= (1 << 30);
        }
        else if (type == TypeValue::Voltage)    // 29-� ��� ����������
        {
            raw |= (1 << 29);
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

    // ������ ��� ����������, ������������ ��� �������
    // MU - ����������
    int ToIntMU() const
    {
        int value = (int)(raw & 0x1FFFFFFF);        // ������� ��� - ����, ��� ��������� - ���

        return (raw & (uint)(1 << 31)) ? -value : value;
    }

    int ToIntAbsMU() const
    {
        int int_value = ToIntMU();

        return int_value >= 0 ? int_value : -int_value;
    }

    // � ������� � � ������
    float ToFloatUnits() const
    {
        return (float)ToIntMU() * 1e-3f;
    }

private:

    // � ���� 31 �������� ���� : 0 - �������������, 1 - �������������
    // ��� 30 - ��� : 0 - ���������� (�), 1 - ����� (��)
    // ���� 0-29 - ��������
    uint raw;
};


struct Voltage : public Value
{
    Voltage(int voltage) : Value(voltage, TypeValue::Voltage) { }

    void Set(int mv)
    {
        Value::Set(mv, TypeValue::Voltage);
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
