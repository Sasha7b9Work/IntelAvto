// 2023/09/08 20:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Display/Display_.h"
#include "Hardware/Keyboard/Keyboard_.h"


void init()
{
    HAL::Init();
    Display::Init();
    Keyboard::Init();
    Menu::Init();
}


void update()
{
    Menu::Input::Update();
    Display::Update();
}
