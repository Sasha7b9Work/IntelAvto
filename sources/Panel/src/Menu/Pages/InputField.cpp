// 2025/04/07 10:05:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/InputField.h"
#include "Display/Primitives_.h"
#include "Display/Text_.h"
#include <cstring>
#include <cstdlib>


using namespace Primitives;


InputField::InputField()
{
    Reset();
}


void InputField::Draw(int x, int y, bool visible)
{
    if (!visible)
    {
        return;
    }

    Rect(80, 30).FillRounded(x, y, 1, Color::BACK, Color::WHITE);

    Font::Set(TypeFont::GOSTB28B);

    Text(buffer).Write(x + 5, y + 2, Color::WHITE);

    Font::Set(TypeFont::GOSTAU16BOLD);
}


void InputField::OnKey(Key::E key)
{
    if (key == Key::Esc)
    {
        Reset();
    }
    else
    {
        if (buffer[0] == 0 && key == Key::_0)
        {
            return;
        }

        if (std::strlen(buffer) < SIZE_BUFFER - 1)
        {
            char symbol = (char)key + 0x30;

            if (key == Key::_0)
            {
                symbol = '0';
            }

            buffer[std::strlen(buffer)] = symbol;
        }
    }
}


void InputField::Reset()
{
    std::memset(buffer, 0, SIZE_BUFFER);
}


int InputField::GetValue() const
{
    return std::atoi(buffer);
}
