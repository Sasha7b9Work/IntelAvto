// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/LAN/LAN_.h"
#include "Menu/Menu.h"
#include "Hardware/Timer.h"
#include "Hardware/USBD/USBD_.h"
#include "Hardware/USBH/USBH_.h"
#include "Display/Display_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "FlashDrive/FlashDrive.h"


int main(void)
{
    HAL::Init();

    Display::Init();

    Keyboard::Init();

    LAN::Init();

    USBD::Init();

    USBH::Init();

    Menu::Init();

    FDrive::Init();

    while (1)
    {
        Menu::Input::Update();
        Display::Update();
        Timer::UpdateTasks();
    }
}
