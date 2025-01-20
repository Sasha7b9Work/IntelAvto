// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Parameter;


struct TypeValue
{
    enum E
    {
        Voltage,    // ������
        Time,       // ������������
        Raw,        // ����������, ��� ������ ���������
        Count
    };
};


struct DrawStruct
{
    void PressKey(int);
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
    Value(int value, TypeValue::E type)
    {
        raw = (uint)value;

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

    Value (uint v) : raw(v) { }

    void Draw(const Parameter *, int x, int y) const;

    static DrawStruct ds;

    // ���������� true, ���� �������� ��������� � �������� [min, max]
    bool FromDrawStrut(const Value &min, const Value &max);

    bool IsRaw() const
    {
        return GetType() == TypeValue::Raw;
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
    int ToInt() const
    {
        int value = (int)(raw & 0x1FFFFFFF);        // ������� ��� - ����, ��� ��������� - ���

        if (raw & (uint)(1 << 31))
        {
            value = -value;
        }

        return value;
    }

    // � ������� � � ������
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

    // � ���� 31 �������� ���� : 0 - �������������, 1 - �������������
    // ��� 30 - ��� : 0 - ���������� (�), 1 - ����� (��)
    // ���� 0-29 - ��������
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


struct ValueNull : public Value
{
    ValueNull() : Value(0, TypeValue::Count) { }
};
