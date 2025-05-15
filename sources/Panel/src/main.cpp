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


#ifndef WIN32
    #if __ARMCC_VERSION != 6210000
        // На других версиях компиляторов не проверялось
        // Но на 6.23 (и, видимо, 6.22) из Keil 5.42a вылетает с BKPT 0xAB. Нужна реализация putc, etc for _sys_open (файл Retarget.c)
        #error "Requires ARM Compiler V6.21 from uVision 5.39"
    #endif
#endif

//__asm(".global __use_no_semihosting");
//
//
//struct __FILE { int handle;} ;
//
//FILE __stdout;
//FILE __stdin;
//FILE __stderr;


int main(void)
{
    HAL::Init();

    gset.Load();
//
    LAN::Init();
    
    Display::Init();

    Keyboard::Init();

    Menu::Init();

//    VCP::Init();

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

        FDrive::Update();

        gset.Save();
    }
}
