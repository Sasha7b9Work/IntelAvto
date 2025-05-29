// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


#ifndef WIN32
    #if __ARMCC_VERSION != 6210000
        // На других версиях компиляторов не проверялось
        // Но на 6.23 из Keil 5.42a не запускается из-за new, malloc
        #error "Requires ARM Compiler V6.21 from uVision 5.39"
    #endif
#endif


#ifdef WIN32
    #define __enable_irq()
    #define __disable_irq()
    #define __set_MSP(x)
    #ifndef __IO
        #define __IO volatile
    #endif
#endif


#define MAIN_PROGRAM_START_ADDRESS  0x8020000


int main()
{    
    CPU::Init();
//
//    Timer::PauseOnTime(250);
//
//    FDrive::Init();
//
//    FDrive::AttemptUpdate();
//
//    Timer::Disable(kTemp);
//
//    CPU::DeInit();

    __disable_irq();

    pFuncVV JumpToApplication;

    JumpToApplication = (pFuncVV)(*(__IO uint *)(0x8020000 + 4)); //-V566

    __set_MSP(*(__IO uint *)0x8020000);

    __enable_irq();
    
    __DSB();
    __ISB();

    JumpToApplication();                                                            //jump to app

    return 0;
}
