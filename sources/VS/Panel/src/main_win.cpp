// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Calculate/ValueFPGA.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Channels/Channels.h"
#include "SCPI/SCPI.h"
#include "Tests.h"


void init()
{
    HAL::Init();
    Display::Init();
    Keyboard::Init();
    Menu::Init();

//    Display::DrawWelcomeScreen();

//    Tests::Run();
}


void update()
{
    ValueFPGA::Create(4999900, 55553, 1660, 5972, 1660);

    Display::Refresh();
    Menu::Input::Update();
    SCPI::Update();
    Display::Update();
}
