// 2023/12/22 14:55:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Text.h"
#include "Utils/StringUtils.h"
#include "Menu/MenuItems.h"
#include <cstring>


DrawStruct Value::ds;


void Value::Draw(const Parameter *param, int x, int y) const
{
    if (param->IsEditable())
    {
        ds.Draw(x, y);
    }
    else
    {
        char string[32];

        char *pointer = string;

        int value = munits;

        if (value < 0)
        {
            value = -value;
            string[0] = '-';
            pointer++;
        }

        *pointer = '\0';

        if (value < 1000)
        {
            std::strcat(pointer, String("%d", value).c_str());
            std::strcat(string, "m");
        }
        else
        {
            int int_value = value / 1000;

            std::strcat(pointer, String("%d", int_value).c_str());

            std::strcat(string, ",");

            value = value - int_value * 1000;

            std::strcat(string, String("%d", value).c_str());
        }

        std::strcat(string, (unit == Unit::Volts) ? "V" : "s");
        Text(string).Write(x, y);
    }
}


void Value::FromDataStruct()
{

}


void DrawStruct::PressKey(Key::E key)
{
    if (key == Key::Minus)
    {
        if (index == 0)
        {
            AppendSymbol('-');
        }
    }
    else if (key == Key::Dot)
    {
        if (!ConsistDot())
        {
            AppendSymbol('.');
        }
    }
    else if (key >= Key::_1 && key <= Key::_0)
    {
        static const char _keys[Key::Count] = { ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

        AppendSymbol(_keys[key]);
    }
    else if (key == Key::Back)
    {
        if (index > 0)
        {
            index--;
        }
    }
}


void DrawStruct::AppendSymbol(char symbol)
{
    if (index < SIZE_BUFER)
    {
        symbols[index++] = symbol;
    }
}


bool DrawStruct::ConsistDot() const
{
    for (int i = 0; i < index; i++)
    {
        if (symbols[i] == '.')
        {
            return true;
        }
    }

    return false;
}


void DrawStruct::Draw(int x, int y) const
{
    Color color = Color::GetCurrent();

    Color::WHITE.SetAsCurrent();

    for (int i = 0; i < index; i++)
    {
        char text[2] = { symbols[i], '\0' };
        Text(text).Write(x + 10 * i, y);
    }

    color.SetAsCurrent();
}
