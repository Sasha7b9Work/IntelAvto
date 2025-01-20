// 2023/12/22 14:55:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Text.h"
#include "Utils/StringUtils.h"
#include "Menu/MenuItems.h"
#include <cstring>


DrawStruct Value::ds;


void Value::Draw(const Parameter *param, int x, int y) const
{
    if (param && param->IsNowEdited())
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
        char string[32];

        char *pointer = string;

        int value = ToInt();

        if (value < 0)
        {
            value = -value;
            string[0] = '-';
            pointer++;
        }

        *pointer = '\0';

        if (value < 1000)
        {
            std::strcat(pointer, Text("%d", value).c_str());

            if (GetType() == TypeValue::Time)
            {
                std::strcat(string, "m");
            }
        }
        else
        {
            int int_value = value / 1000;

            std::strcat(pointer, Text("%d", int_value).c_str());

            std::strcat(string, ",");

            value = value - int_value * 1000;

            std::strcat(string, Text("%d", value).c_str());
        }

        if (GetType() != TypeValue::Raw)
        {
            std::strcat(string, (GetType() == TypeValue::Time) ? "s" : "V");
        }
        Text(string).Write(x, y);
    }
}


bool DrawStruct::ToRaw(uint *result, TypeValue::E type) const
{
    char buffer[128];

    std::memcpy(buffer, symbols, (uint)index);

    buffer[index] = '\0';

    char *end = nullptr;

    int value = 0;

    if (SU::String2Int(buffer, &value, &end))
    {
        *result = (value > 0) ? (uint)value : (uint)-value;

        if (value < 0)
        {
            *result |= (uint)(1 << 31);
        }

        if (type == TypeValue::Time)
        {
            *result |= (1 << 30);
        }
        else if (type == TypeValue::Voltage)
        {
            *result |= (1 << 29);
        }

        return true;
    }

    return false;
}


bool Value::FromDrawStrut(const Value &min, const Value &max)
{
    uint new_raw = 0;

    bool result = false;

    if (ds.ToRaw(&new_raw, GetType()))
    {
        Value value(new_raw);

        if (value.ToFloat() >= min.ToFloat() && value.ToFloat() <= max.ToFloat())
        {
            raw = new_raw;

            result = true;
        }
    }

    return result;
}


void DrawStruct::PressKey(int _key)
{
    Key::E key = (Key::E)_key;

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
    else if (key >= Key::_1 && key <= Key::_0)
    {
        static const char _keys[Key::Count] = { ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

        AppendSymbol(_keys[key]);
    }
//    else if (key == Key::Back)
//    {
//        if (index > 0)
//        {
//            index--;
//        }
//    }
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
