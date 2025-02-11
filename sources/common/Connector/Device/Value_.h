// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Parameter;
struct Value;


struct TypeValue
{
    enum E
    {
        Voltage,    // ������
        Time,       // ������������
        Raw         // ����������, ��� ������ ���������
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
    int index;                          // ������� ������. 0 - "-", ���� ����
    static const int SIZE_BUFER = 10;   // ������������ ���������� ������� �������� ������ � ����������� ����
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
    int ToInt() const
    {
        int value = (int)(raw & 0x1FFFFFFF);        // ������� ��� - ����, ��� ��������� - ���

        return (raw & (uint)(1 << 31)) ? -value : value;
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


struct ValueNull : public Counter
{
    ValueNull() : Counter(0) { }
};
