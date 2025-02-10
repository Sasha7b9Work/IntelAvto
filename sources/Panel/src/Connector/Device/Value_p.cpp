// 2023/12/22 14:55:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Display/Text_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "Utils/StringUtils_.h"
#include "Display/Primitives_.h"
#include <cstring>
#include <cstdlib>


using namespace Primitives;


void Value::Draw(int x, int y) const
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

    if (value < 1000 || IsRaw())
    {
        std::strcat(pointer, Text("%d", value).c_str());

        if (GetType() == TypeValue::Time)
        {
            std::strcat(string, "м");
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
        std::strcat(string, (GetType() == TypeValue::Time) ? "с" : "В");
    }
    Text(string).Write(x, y, Color::WHITE);
}


bool DrawStruct::ToValue(Value *result, TypeValue::E type) const
{
    char buffer[128];

    std::memcpy(buffer, symbols, std::strlen(symbols));

    char *end = nullptr;

    int value = 0;

    if (SU::String2Int(buffer, &value, &end))
    {
        uint raw = (value > 0) ? (uint)value : (uint)-value;

        if (value < 0)
        {
            raw |= (uint)(1 << 31);
        }

        if (type == TypeValue::Time)
        {
            raw |= (1 << 30);
        }
        else if (type == TypeValue::Voltage)
        {
            raw |= (1 << 29);
        }

        *result = Value(raw);

        return true;
    }

    return false;
}


void DrawStruct::PressKey(int _key)
{
    Key::E key = (Key::E)_key;

    if (key == Key::Minus)
    {
        if (parameter->GetValue().IsVoltage() && index == 0)
        {
            SetSymbolToCurrentPos('-');
        }
    }
    else if (key == Key::Left)
    {
        if (index > 0)
        {
            index--;
        }
    }
    else if (key == Key::Right)
    {
        if (index < (int)std::strlen(symbols) - 1)
        {
            index++;
        }
    }
    else if (key >= Key::_1 && key <= Key::_0)
    {
        if ((key == Key::_0 && index == 0) ||
            (key == Key::_0 && index == 1 && symbols[0] == '-'))
        {
            // Ноль первым быть не может
        }
        else
        {
            static const char _keys[Key::Count] = { ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' };

            SetSymbolToCurrentPos(_keys[key]);
        }
    }
}


void DrawStruct::SetSymbolToCurrentPos(char symbol)
{
    if (index < SIZE_BUFER - 1)
    {
        symbols[index++] = symbol;
    }
}


bool DrawStruct::ConsistDot() const
{
    for (uint i = 0; i < std::strlen(symbols); i++)
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
    for (int i = 0; i < (int)std::strlen(symbols); i++)
    {
        Rect(13, 18).Fill(x + 10 * i - 2, y - 2, (i == index) ? Color::BLUE : Color::BACK);

        char text[2] = { symbols[i], '\0' };
        Text(text).Write(x + 10 * i, y, Color::WHITE);
    }
}


void DrawStruct::Set(Parameter *_param)
{
    parameter = _param;

    Math::ItoA(parameter->GetValue().ToInt(), symbols);

    index = (int)std::strlen(symbols) - 1;
}
