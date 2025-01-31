// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Parameter;
struct Value;


enum TypeValue
{
    TVolt,      // ������
    TTime,      // ������������
    TRaw        // ����������, ��� ������ ���������
};


struct DrawStruct
{
    void PressKey(int);
    void Draw(int x, int y) const;
    void Clear(Parameter *_param) { index = 0; parameter = _param; }
    bool ToValue(Value *, TypeValue) const;
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
    Value(int value, TypeValue type)
    {
        raw = value > 0 ? (uint)value : (uint)(-value);

        if (value < 0 && type != TRaw)
        {
            raw |= (uint)(1 << 31);
        }

        if (type == TRaw)             // 29-� � 30-� ���� ������
        {

        }
        else if (type == TTime)       // 30-� ��� ����������
        {
            raw |= (1 << 30);
        }
        else if (type == TVolt)    // 29-� ��� ����������
        {
            raw |= (1 << 29);
        }
    }

    void Draw(const Parameter *, int x, int y) const;

    static DrawStruct ds;

    // ������������� ��������, ��������� ����������� � ������������ ����������
    void SetValue(const Value &min, const Value &max);

    bool IsRaw() const
    {
        return GetType() == TRaw;
    }

    bool IsVoltage() const
    {
        return GetType() == TVolt;
    }

    TypeValue GetType() const
    {
        if (raw & (1 << 30))
        {
            return TTime;
        }

        if (raw & (1 << 29))
        {
            return TVolt;
        }

        return TRaw;
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

        if (GetType() == TTime)
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
    Voltage(int voltage) : Value(voltage, TVolt) { }
};


struct Time : public Value
{
    Time(int ms) : Value(ms, TTime) { }
};


struct Counter : public Value
{
    Counter(uint n) : Value((int)n, TRaw) { }
};


struct ValueNull : public Value
{
    ValueNull() : Value(0, TRaw) { }
};
