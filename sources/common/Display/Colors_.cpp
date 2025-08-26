// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"


Color Color::FILL(0);
Color Color::BACK(1);
Color Color::MENU_UNSELECT(2);
Color Color::MENU_SELECT(3);
Color Color::RED(4);
Color Color::GREEN(5);
Color Color::BLUE(6);
Color Color::WHITE(7);
Color Color::BLACK(8);
Color Color::GRAY(9);
Color Color::YELLOW(10);
Color Color::GRAY_75(11);


namespace ColorTimer
{
    static uint time_reset = 0;
}


uint &Color::Value() const
{
    return ColorScheme::Current().colors[index];
}


ColorScheme &ColorScheme::Current()
{
    return gset.schemes[gset.colorScheme];
}


uint8 Color::GetRED() const
{
    return (uint8)(Value() >> 16);
}


uint8 Color::GetGREEN() const
{
    return (uint8)(Value() >> 8);
}


uint8 Color::GetBLUE() const
{
    return (uint8)(Value());
}


void ColorTimer::Reset()
{
    time_reset = TIME_MS;
}


bool ColorTimer::IsMain()
{
    return (((TIME_MS - time_reset) / 1000) % 2) == 0;
}
