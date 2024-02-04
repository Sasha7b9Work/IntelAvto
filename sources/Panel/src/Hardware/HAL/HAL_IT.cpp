// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"
#include <stm32f4xx_hal.h>


#ifdef __cplusplus
extern "C" {
#endif


void NMI_Handler(void)
{

}

__attribute((noreturn)) void HardFault_Handler(void)
{
    pchar file = Debug::file[0];
    int line = Debug::line[0];
    
    (void)file;
    (void)line;

    while (1) //-V776
    {
        file = Debug::file[0];
        line = Debug::line[0];
        
        file = Debug::file[0]; //-V570 //-V1048
        line = Debug::line[0]; //-V570 //-V1048
    }
}

__attribute((noreturn)) void MemManage_Handler(void)
{
    while (1)
    {
    }
}

__attribute((noreturn)) void BusFault_Handler(void)
{
    while (1)
    {
    }
}

__attribute((noreturn)) void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

void SVC_Handler(void)
{

}

void DebugMon_Handler(void)
{

}

void PendSV_Handler(void)
{

}

void SysTick_Handler(void)
{
    HAL_IncTick();
}


void OTG_FS_IRQHandler()
{
//    HAL_PCD_IRQHandler((PCD_HandleTypeDef *)HAL_PCD::handle);
}


#ifdef __cplusplus
}
#endif
