// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Keyboard/Keyboard.h"


class Parameter;


enum TypeValue
{
    Volts,
    MS
};


struct DrawStruct
{
    void PressKey(Key::E);
    void Draw(int x, int y) const;
    void Clear(Parameter *_param) { index = 0; parameter = _param; }
    bool ToRaw(uint *result, TypeValue) const;
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
    Value(uint _raw) : raw(_raw) { }

    Value(int value = 0, TypeValue type = MS)
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

    // ���������� true, ���� �������� ��������� � �������� [min, max]
    bool FromDrawStrut(const Value &min, const Value &max);

    TypeValue GetType() const
    {
        return ((raw & (1 << 30)) == 0) ? Volts : MS;
    }

    uint GetRaw() const { return raw; }

    // ������ ��� ����������, ������������ ��� �������
    int ToInt() const
    {
        int value = (int)(raw & 0x3FFFFFFF);

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

        if (GetType() == MS)
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
    Voltage(int voltage) : Value(voltage, TypeValue::Volts) {}
};


struct Time : public Value
{
    Time(int ms) : Value(ms, TypeValue::MS) {}
};
