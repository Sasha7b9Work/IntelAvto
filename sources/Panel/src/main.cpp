// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Log.h"
#include "Display/Display.h"
#include "Hardware/FPGA.h"
#include "Hardware/VCP.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "SCPI/SCPI.h"
#include "Tests.h"
#include "Settings.h"


int main(void)
{
    HAL::Init();

    glob_set.Load();

    FPGA::Init();
    Display::Init();

    if (Keyboard::Init())
    {
        Display::DrawKeyboardFailScreen();
        HAL_TIM::DelayMS(5000);
    }

    VCP::Init();
    Menu::Init();

    //    Tests::Run();

    Display::DrawWelcomeScreen();

    while (1)
    {
        FPGA::Update();
        Menu::Input::Update();
        SCPI::Update();
        Display::Update();
    }
}
