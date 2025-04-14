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

    // Текущее значение является минимально допустимым
    bool IsMinimalValueOrLess() const;

    // Текущее значение является максимально допустимым
    bool IsMaximumValueOrAbove() const;

    Parameter *parameter;

    struct Params
    {
        static const int SIZE_BUFER = 10;   // Максимальное количество хранимх разрядов вместе с завершающим нулём
        int index;                          // Текущий разряд. 0 - "-", если есть
        char symbols[SIZE_BUFER];
        bool is_negative = false;           // Полярность значения

        bool OnLeftAllNines(int pos);

        bool IsDigit(char) const;

        void IncreaseInPosition(int);

        void DecreaseInPosition(int);

        int NumSymbols() const;

        void SetSymbolToCurrentPos(char);

        void Set(const Value &);
    } p;
};
