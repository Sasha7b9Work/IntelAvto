// 2022/01/17 16:11:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/FDrive.h"
#include <usbh_core.h>
#include <stm32f4xx_hal.h>


#ifdef __cplusplus
extern "C" {
#endif

    void SysTick_Handler()
    {
        HAL_IncTick();
        HAL_SYSTICK_IRQHandler();
    }


    // This interrupt call soft NSS for spi (see Hardware::SPIforPanel.c::PanelInit() and HAL_GPIO_EXTI_Callback())
    void EXTI9_5_IRQHandler()
    {
        HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    }



    void OTG_FS_IRQHandler()
    {
        HAL_HCD_IRQHandler(reinterpret_cast<HCD_HandleTypeDef *>(FDrive::GetHandleHCD()));
    }

#ifdef __cplusplus
}
#endif
