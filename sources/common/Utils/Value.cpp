// 2023/12/22 14:55:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Value.h"
#include "Display/Text.h"
#include "Utils/StringUtils.h"
#include "Menu/MenuItems.h"
#include <cstring>


DrawStruct Value::ds;


void Value::Draw(const Parameter *param, int x, int y) const
{
    if (param->IsEditable())
    {
        ds.Draw();
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


void DrawStruct::PressKey(Key::E)
{

}


void DrawStruct::Draw() const
{

}
