// 2023/12/22 14:55:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Display/Text_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "Utils/StringUtils_.h"
#include <cstring>


DrawStruct Value::ds;


void Value::Draw(const Parameter *param, int x, int y) const
{
    if (param && param->IsNowEdited())
    {
        ds.Draw(x, y);

        Color color = Color::GetCurrent();

        Color::BLUE.SetAsCurrent();

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
}


bool DrawStruct::ToValue(Value *result, TypeValue::E type) const
{
    char buffer[128];

    std::memcpy(buffer, symbols, (uint)index);

    buffer[index] = '\0';

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

        *result = Value((int)raw, TypeValue::Raw);

        return true;
    }

    return false;
}


void Value::SetValue(const Value &min, const Value &max)
{
    Value new_value(0, TypeValue::Raw);

    if (ds.ToValue(&new_value, GetType()))
    {
        float value = new_value.ToFloat();
        float min_val = min.ToFloat();
        float max_val = max.ToFloat();

        if (value < min_val)
        {
            raw = min.GetRaw();
        }
        else if (value > max_val)
        {
            raw = max.GetRaw();
        }
        else
        {
            raw = new_value.GetRaw();
        }
    }
}


void DrawStruct::PressKey(int _key)
{
    Key::E key = (Key::E)_key;

    if (key == Key::Minus)
    {
        if (parameter->GetValue().IsVoltage() && index == 0)
        {
            AppendSymbol('-');
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

            AppendSymbol(_keys[key]);
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
