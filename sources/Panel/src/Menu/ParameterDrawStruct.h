// 2025/02/13 10:50:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Device/Value_.h"

class Parameter;


struct ParameterDrawStruct
{
    void PressKey(int);

    void Draw(int x, int y) const;

    void Set(Parameter *);

    bool ToValue(Value *) const;

private:

    void IncreaseInPosition(int);

    void DecreaseInPosition(int);

    // ������� �������� �������� ���������� ����������
    bool IsMinimalValueOrLess() const;

    // ������� �������� �������� ����������� ����������
    bool IsMaximumValueOrAbove() const;

    Parameter *parameter;

    struct Params
    {
        static const int SIZE_BUFER = 10;   // ������������ ���������� ������� �������� ������ � ����������� ����
        int index;                          // ������� ������. 0 - "-", ���� ����
        char symbols[SIZE_BUFER];
        bool is_negative = false;           // ���������� ��������

        Value value{ 0, TypeValue::Raw };

        bool OnLeftAllNines(int pos);

        bool IsDigit(char) const;

        void IncreaseInPosition(int);

        void DecreaseInPosition(int);

        int NumSymbols() const;

        void SetSymbolToCurrentPos(char);

        void Set(const Value &);
    } p;
};
