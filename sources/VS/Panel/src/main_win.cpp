// 2023/09/08 20:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"


void init()
{
    HAL::Init();
    Display::Init();
    Keyboard::Init();
    Menu::Init();
}


void update()
{
    Display::Refresh();
    Menu::Input::Update();
    Display::Update();
}
