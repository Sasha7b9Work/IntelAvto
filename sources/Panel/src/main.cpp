// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Connector/Messages_.h"
#include "Menu/Pages/Pages.h"


int main(void)
    {
    HAL::Init();

    Display::Init();

    Keyboard::Init();

    Menu::Init();

    while (1)
    {
        Menu::Input::Update();
        Display::Update();
    }
}
