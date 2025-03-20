// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Hardware/Timer.h"
#include "Display/Display_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "FlashDrive/FlashDrive.h"
#include "LAN/LAN.h"
#include "Device/IT6523.h"
#include "VCP/VCP.h"


int main(void)
{
    HAL::Init();

    LAN::Init();
    
    Display::Init();

    Keyboard::Init();

    Menu::Init();

    VCP::Init();

    FDrive::Init();

    while (1)
    {
        LAN::Update();
        Menu::Input::Update();
        LAN::Update();
        Display::Update();
        LAN::Update();
        Timer::UpdateTasks();
        LAN::Update();
        IT6523::Update();
    }
}
