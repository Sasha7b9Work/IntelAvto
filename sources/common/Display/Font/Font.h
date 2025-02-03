// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "FontBig.h"
#include "FontMid.h"


struct TypeFont
{
    enum E
    {
        GOSTAU16BOLD,
        GOSTB28B,
        Count,
        None
    } value;
};


namespace Font
{
    // Устанавливает текущий шрифт. Ранее установленный можно восстановить функцией Pop()
    void Set(const TypeFont::E);
    uint8 GetWidth(uint8 symbol);
    uint8 GetWidth(char symbol);
    uint8 GetHeight();
    uint8 GetHeight(uint8 symbol);
    bool RowNotEmpty(uint8 symbol, int row);
    bool BitIsExist(uint8 symbol, int row, int bit);
    // Устанавливает количество пустых пикселей между символами.
    void SetSpacing(int spacing);
    int GetSpacing();
    int GetLengthSymbols(pchar start, int num);
    int GetLengthText(pchar text);
};
