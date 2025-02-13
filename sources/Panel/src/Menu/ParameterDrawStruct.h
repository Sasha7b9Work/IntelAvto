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

    bool ConsistDot() const;

    void SetSymbolToCurrentPos(char);

    int NumSymbols() const;

    void IncreaseInPosition(int);

    void DecreaseInPosition(int);

    bool OnLeftAllNines(int pos);

    bool IsDigit(char) const;

    // ������� �������� �������� ���������� ����������
    bool IsMinimalValue() const;

    // ������� �������� �������� ����������� ����������
    bool IsMaximumValue() const;

    Parameter *parameter;

    int index;                          // ������� ������. 0 - "-", ���� ����

    static const int SIZE_BUFER = 10;   // ������������ ���������� ������� �������� ������ � ����������� ����

    char symbols[SIZE_BUFER];

    bool is_negative = false;           // ���������� ��������
};



