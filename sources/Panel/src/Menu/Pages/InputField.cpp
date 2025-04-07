// 2025/04/07 10:05:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/InputField.h"
#include "Display/Primitives_.h"
#include "Display/Text_.h"
#include <cstring>


using namespace Primitives;


InputField::InputField()
{
    std::memset(buffer, 0, SIZE_BUFFER);
}


void InputField::Draw(int x, int y, bool visible)
{
    if (!visible)
    {
        return;
    }

    Rect(150, 30).FillRounded(x, y, 1, Color::BACK, Color::WHITE);

    Text(buffer).Write(x + 5, y + 2, Color::WHITE);
}
