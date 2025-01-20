// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Colors.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Settings/Settings.h"


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


uint &Color::Value() const
{
    return ColorScheme::Current().colors[index];
}


ColorScheme &ColorScheme::Current()
{
    return gset.schemes[gset.colorScheme];
}
