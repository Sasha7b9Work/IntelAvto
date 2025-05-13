// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Hardware/Timer.h"
#include "Display/Display_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "FlashDrive/FlashDrive.h"
#include "Device/IT6523.h"
#include "VCP/VCP.h"


#ifndef WIN32
    #if __ARMCC_VERSION != 6230001
        // На других версиях компиляторов не проверялось
        #error "Requires ARM Compiler V6.23 from uVision 5.42"
    #endif
#endif




int main(void)
{
    HAL::Init();

//    gset.Load();
//
//    LAN::Init();
//    
//    Display::Init();
//
//    Keyboard::Init();
//
//    Menu::Init();
//
////    VCP::Init();
//
//    FDrive::Init();
//
//    while (1)
//    {
//        LAN::Update();
//        Menu::Input::Update();
//
//        LAN::Update();
//        Display::Update();
//
//        LAN::Update();
//        Timer::UpdateTasks();
//
//        LAN::Update();
//        IT6523::Update();
//
//        FDrive::Update();
//
//        gset.Save();
//    }
}
