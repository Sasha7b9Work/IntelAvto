// 2025/02/13 10:50:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

class Parameter;
struct Value;


struct ParameterDrawStruct
{
    void PressKey(int);

    void Draw(int x, int y) const;

    void Set(Parameter *_param);

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

        void PressKey(int, Parameter *);

        bool OnLeftAllNines(int pos);

        bool IsDigit(char) const;

        void IncreaseInPosition(int);

        void DecreaseInPosition(int);

        int NumSymbols() const;

        void SetSymbolToCurrentPos(char);

        void Set(int);
    } p;
};
