// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItems.h"
#include <cstring>


using namespace Primitives;



Text::Text(pchar t)
{
    if (t == nullptr)
    {
        (void)t;
    }

    Create(t);
}


Text::Text(const String &t)
{
    Create(t.c_str());
}


Text::~Text()
{
    delete []text;
}


void Text::Create(pchar _text)
{
    uint numSymbols = std::strlen(_text);

    if (numSymbols)
    {
        text = new char[numSymbols + 1];
        std::strcpy(text, _text);
    }
    else
    {
        text = nullptr;
    }
}


int Text::Write(int x, int y, Color color)
{
    color.SetAsCurrent();

    return Write(x, y);
}


int Text::Write(int x, int y)
{
    if (text)
    {
        x = WriteSymbols(text, (int)std::strlen(text), x, y);
    }

    return x;
}


int Text::WriteScaled(int x, int y, int size)
{
    if (text)
    {
        x = WriteSymbolsScaled(text, (int)std::strlen(text), x, y, size);
    }

    return x;
}


int Text::WriteSymbols(char* start, int num, int x, int y) const
{
    if (Display::InDrawingPart(y, Font::GetHeight()))
    {
        char *p = start;

        for (int i = 0; i < num; i++)
        {
            x = WriteSymbol(x, y, (uint8)(*p++)) + Font::GetSpacing();
        }
    }

    return x;
}


int Text::WriteSymbolsScaled(char *start, int num, int x, int y, int size) const
{
    if (Display::InDrawingPart(y, Font::GetHeight() * size))
    {
        char *p = start;

        for (int i = 0; i < num; i++)
        {
            x = WriteSymbolScaled(x, y, (uint8)(*p++), size) + Font::GetSpacing() * size;
        }
    }

    return x;
}


int Text::Write(int x, int y, int _width, Color color)
{
    color.SetAsCurrent();

    return Write(x, y, _width);
}


int Text::Write(int x, int y, int width)
{
    if (Display::InDrawingPart(y, Font::GetHeight()))
    {
        int length = Font::GetLengthText(text);

        int delta = (width - length) / 2;

        x = Write(x + delta, y);
    }

    return x;
}


void Text::WriteSymbols(char *start, int num, int x, int y, int width, Color color) const
{
    color.SetAsCurrent();

    WriteSymbols(start, num, x, y, width);
}


void Text::WriteSymbols(char *start, int num, int x, int y, int width) const
{
    int length = Font::GetLengthSymbols(start, num);

    int delta = (width - length) / 2;

    WriteSymbols(start, num, x + delta, y);
}


void Text::WriteInCenterRect(int x, int y, int width, int height, Color color)
{
    color.SetAsCurrent();

    WriteInCenterRect(x, y, width, height);
}


void Text::WriteInCenterRect(int x, int y, int width, int height)
{
    volatile int numWords = NumWords();

    if (numWords == 1)
    {
        int dY = (height - Font::GetHeight()) / 2;
        Write(x, y + dY, width);
    }
    else if (numWords == 2)
    {
        char *start = nullptr;
        int num = 0;

        GetWord(0, start, num);

        int dY = (height - Font::GetHeight()) / 2 - Font::GetHeight() / 2 - 1;

        WriteSymbols(start, num, x, y + dY, width);

        GetWord(1, start, num);

        WriteSymbols(start, num, x, y + 6 + dY + static_cast<int>(1.0F * (float)Font::GetHeight()), width);
    }
    else
    {
        // остальные варианты пока не трогаем
    }
}


// Пропускает пробелы
static void SkipSpaces(char** p)
{
    while (true)
    {
        if (**p == 0)
        {
        }
        else if (**p == ' ')
        {
            (*p)++;
            continue;
        }
        else
        {
            // остальные варианты не рассматриваем
        }

        break;
    }
}


// Пропускает символы
static void SkipSymbols(char** p)
{
    while (true)
    {
        if (**p == 0)
        {
        }
        else if (**p != ' ')
        {
            (*p)++;
            continue;
        }
        else
        {
            // остальные варианты не рассматриваем
        }

        break;
    }
}


int Text::NumWords() const
{
    int result = 0;

    char* p = text;

    bool run = true;

    while (run)
    {
        SkipSpaces(&p);

        if (*p == 0)
        {
            run = false;
            continue;
        }

        result++;

        SkipSymbols(&p);

        if (*p == 0)
        {
            run = false;
            continue;
        }
    }

    return result;
}


void Text::GetWord(int numWord, char *(&start), int &num)
{
    char *p = text;

    int word = 0;

    while (true) //-V2530
    {
        num = 0;
        start = nullptr;

        SkipSpaces(&p);

        if (*p == 0)
        {
            num = 0;
            start = nullptr;
            break;
        }

        start = p;

        while ((*p != 0) && *p != ' ')
        {
            num++;
            p++;
        }

        if (numWord == word)
        {

            break;
        }

        word++;
    }
}


int Text::WriteSymbol(int x, int y, uint8 chr) const
{
    int height = Font::GetHeight();
    int width = Font::GetWidth(chr);
    
    for (int row = 0; row < height; row++)
    {
        if (Font::RowNotEmpty(chr, row))
        {
            for (int col = 0; col < width; col++)
            {
                if (Font::BitIsExist(chr, row, col))
                {
                    Point().Draw(x + col, y + row);
                }
            }
        }
    }

    return x + width;
}


int Text::WriteSymbolScaled(int x, int y, uint8 chr, int size) const
{
    int height = Font::GetHeight();
    int width = Font::GetWidth(chr);

    for (int row = 0; row < height; row++)
    {
        if (Font::RowNotEmpty(chr, row))
        {
            for (int col = 0; col < width; col++)
            {
                if (Font::BitIsExist(chr, row, col))
                {
                    Rectangle(size, size).Fill(x + col * size, y + row * size);
                }
            }
        }
    }

    return x + width * size;
}
