// 2023/12/22 14:55:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Text.h"
#include "Utils/StringUtils.h"
#include "Menu/MenuItems.h"
#include <cstring>


DrawStruct Value::ds;


void Value::Draw(const Parameter *param, int x, int y) const
{
    if (param && param->IsEditable())
    {
        ds.Draw(x, y);

        Color color = Color::GetCurrent();

        Color::TYPE_BLUE.SetAsCurrent();

        param->GetMin().Draw(nullptr, x, y + 25);

        param->GetMax().Draw(nullptr, x, y - 25);

        color.SetAsCurrent();
    }
    else
    {
        if (unit == Unit::Raw)
        {
            Text(String("%d", munits)).Write(x, y);
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

                if (unit == Unit::Seconds)
                {
                    std::strcat(string, "m");
                }
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
}


bool DrawStruct::ToMicroUnits(int *result) const
{
    char buffer[128];

    std::memcpy(buffer, symbols, (uint)index);

    buffer[index] = '\0';

    char *end = nullptr;

    return SU::String2Int(buffer, result, &end);
}


void Value::FromDrawStrut(const Value &min, const Value &max)
{
    int new_microunits = 0;

    if (ds.ToMicroUnits(&new_microunits))
    {
        Value value(new_microunits);

        if (value.ToFloat() < min.ToFloat())
        {
            return;
        }

        if (value.ToFloat() > max.ToFloat())
        {
            return;
        }

        munits = new_microunits;
    }
}


void DrawStruct::PressKey(Key::E key)
{
    if (key == Key::Minus)
    {
        if (parameter->GetMin().ToFloat() < 0.0f && index == 0)
        {
            AppendSymbol('-');
        }
        else
        {
            AppendSymbol('0');
        }
    }
//    else if (key == Key::Dot)
//    {
//        if (!ConsistDot())
//        {
//            AppendSymbol('.');
//        }
//    }
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


float Value::ToFloat() const
{
    return (float)munits / 1000.0f;
}
