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

    // Текущее значение является минимально допустимым
    bool IsMinimalValue() const;

    // Текущее значение является максимально допустимым
    bool IsMaximumValue() const;

    Parameter *parameter;

    int index;                          // Текущий разряд. 0 - "-", если есть

    static const int SIZE_BUFER = 10;   // Максимальное количество хранимх разрядов вместе с завершающим нулём

    char symbols[SIZE_BUFER];

    bool is_negative = false;           // Полярность значения
};



