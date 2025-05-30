// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Font/Font.h"
#include "Display/Font/fontGOSTB28B.inc"
#include "Display/Font/fontGOSTAU16BOLD.inc"
#include "Hardware/HAL/HAL.h"
#include <cstring>


struct NativeSymbol
{
    uint8 width;        // ������ ������� � ��������
    uint8 height;       // ������ ������� � ��������
    uint8 firstRow;     // ����� ������ �������� ������. ������ � ������ ���� �������� � data

    // ���������� ���������� ���� � ������
    int BytesInRow()
    {
        int result = width / 8;

        if (width % 8)
        {
            result++;
        }

        return result;
    }
    // ���������� ��������� �� ������ ���� ������
    uint8 *GetRow(int row)
    {
        if (row > height - 1)
        {
            return nullptr;
        }

        if (row < firstRow)
        {
            return nullptr;
        }

        return Data() + (row - firstRow) * BytesInRow();
    }
    // ���������� ��������� �� ������ ���� ������
    uint8 *Data();

    bool BitIsExist(int r, int b)
    {
        uint8 *row = GetRow(r);

        if (row == nullptr)
        {
            return false;
        }

        while (b > 7)       // ������������� � �����, ����������� ��� ���
        {
            row++;
            b -= 8;
        }

        return ((*row) & (1 << (7 - b))) != 0;
    }
};


// ���������� ���������
struct HeaderFont
{
    uint16       offsets[256];  // �������� 256 �������� �������. 0 ��������, ��� ������ �����������
    NativeSymbol symbol;        // ������ ������ � ������� ��� �������� 256

    // ���������� ��������� �� ������, ���� �� ������������ � ������� � nullptr � �������� ������
    NativeSymbol *GetSymbol(uint8 num)
    {
        HeaderFont *header = HeaderFont::Sefl();

        if (header->offsets[num] == 0)
        {
            return nullptr;
        }

        uint8 *offset = reinterpret_cast<uint8 *>(header) + header->offsets[num];

        return reinterpret_cast<NativeSymbol *>(offset);
    }

    static HeaderFont *Sefl();
};

namespace Font
{
    static int size = 1;                // ������ ����� ������

    static int spacing = 1;

    static const unsigned char *font = nullptr;
}


void Font::SetSize(int s)
{
    size = s;
}


int Font::GetSize()
{
    return size;
}


HeaderFont *HeaderFont::Sefl()
{
    return reinterpret_cast<HeaderFont *>(const_cast<uint8 *>(Font::font)); //-V2567
}


uint8 *NativeSymbol::Data()
{
    return reinterpret_cast<uint8 *>(this) + sizeof(*this);
}


void Font::Set(const TypeFont::E typeFont)
{
    switch (typeFont)
    {
    case TypeFont::GOSTAU16BOLD:  font = fontGOSTAU16BOLD;   break;
    case TypeFont::GOSTB28B:      font = fontGOSTB28B;       break;
    case TypeFont::None:
    case TypeFont::Count:
        break;
    }
}


TypeFont::E Font::Get()
{
    if (font == fontGOSTAU16BOLD)
    {
        return TypeFont::GOSTAU16BOLD;
    }
    else if (font == fontGOSTB28B)
    {
        return TypeFont::GOSTB28B;
    }

    return TypeFont::Count;
}


uint8 Font::GetWidth(uint8 num)
{
    if (num == 0x20)
    {
        if (font == fontGOSTAU16BOLD)
        {
            return 5;
        }
    }

    if (num == '1')
    {
        return GetWidth('0');
    }

    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(num);

    return symbol ? symbol->width : 0U;
}


uint8 Font::GetWidth(char symbol)
{
    return GetWidth(static_cast<uint8>(symbol));
}


uint8 Font::GetHeight()
{
    uint8 result = 0;

    for (int i = 0; i < 256; i++)
    {
        NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(static_cast<uint8>(i));

        if (symbol && symbol->height > result)
        {
            result = symbol->height;
        }
    }

    return result;
}


uint8 Font::GetHeight(uint8 s)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(s);

    return symbol ? symbol->height : 0U;
}


bool Font::RowNotEmpty(uint8 s, int r)
{
    HeaderFont *header = HeaderFont::Sefl();

    NativeSymbol *symbol = header->GetSymbol(s);

    if (symbol)
    {
        if (r < symbol->firstRow)
        {
            return false;
        }

        uint8 *row = symbol->GetRow(r);

        if (row)
        {
            for (int i = 0; i < symbol->BytesInRow(); i++)
            {
                if (row[i] != 0)
                {
                    return true;
                }
            }
        }
    }

    return false;
}


bool Font::BitIsExist(uint8 s, int row, int bit)
{
    NativeSymbol *symbol = HeaderFont::Sefl()->GetSymbol(s);

    return symbol ? symbol->BitIsExist(row, bit) : false;
}


void Font::SetSpacing(int s)
{
    spacing = s;
}


int Font::GetSpacing()
{
    return spacing;
}


int Font::GetLengthSymbols(pchar start, int num)
{
    int result = 0;

    for (int i = 0; i < num; i++)
    {
        result += GetWidth(*start++);
        result += spacing;
    }

    if (result > spacing)
    {
        result -= spacing;
    }

    return result;
}


int Font::GetLengthText(pchar text)
{
    return GetLengthSymbols(text, (int)std::strlen(text));
}
