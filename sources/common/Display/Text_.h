// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Font/Font.h"


struct Color;


class Text
{
public:
    explicit Text(pchar format, ...);
    ~Text();
    int Write(int x, int y, const Color &);
    int Write(int x, int y);
    int Write(int x, int y, int width, const Color &);
    int Write(int x, int y, int width);
    int WriteScaled(int x, int y, int size);
    void WriteInCenterRect(int x, int y, int width, int height, const Color &);
    void WriteInCenterRect(int x, int y, int width, int height);
    pchar c_str() const
    {
        return text;
    }
private:
    char *text = nullptr;

    void Create(pchar text);

    int WriteSymbol(int x, int y, uint8 symbol) const;
    int WriteSymbolScaled(int x, int y, uint8 symbol, int scale) const;

    // Возвращает количество слов в строке
    int NumWords() const;

    // Возвращает информацию о слове numWord
    void GetWord(int numWord, char *(&start), int &num);

    // Нарисовать num символов, начиная с адреса start
    int WriteSymbols(char *start, int num, int x, int y) const;

    int WriteSymbolsScaled(char *start, int num, int x, int y, int size) const;

    // Нарисовать num символов, начиная с адреса старт, в поле шириной width символов
    void WriteSymbols(char *start, int num, int x, int y, int width, const Color &) const;

    void WriteSymbols(char *start, int num, int x, int y, int width) const;

    void Clear();
};
