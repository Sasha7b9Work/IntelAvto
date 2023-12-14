// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Log.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "Settings.h"


int main(void)
{
    HAL::Init();

    gset.Load();

    Display::Init();

    if (Keyboard::Init())
    {
        Display::DrawKeyboardFailScreen();
        HAL_TIM::DelayMS(5000);
    }

    Menu::Init();

    while (1)
    {
        Menu::Input::Update();
        Display::Update();
    }
}
