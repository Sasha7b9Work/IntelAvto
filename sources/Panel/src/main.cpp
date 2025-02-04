// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Hardware/Timer.h"
#include "Display/Display_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "FlashDrive/FlashDrive.h"
#include "LAN/LAN_.h"


int main(void)
{
    HAL::Init();

    Display::Init();

    Keyboard::Init();

    Menu::Init();

//    FDrive::Init();

//    LAN::Init();

    while (1)
    {
        Menu::Input::Update();
        Display::Update();
        Timer::UpdateTasks();
    }
}
