// 2024/02/08 21:13:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL_PINS.h"
#include <stm32f4xx_hal.h>


void PinOut::Init()
{
    GPIO_InitTypeDef is =
    {
        pin,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP,
        GPIO_SPEED_FREQ_VERY_HIGH,
        0
    };

    HAL_GPIO_Init((GPIO_TypeDef *)port, &is);
}


void PinOut::ToLow()
{
    HAL_GPIO_WritePin((GPIO_TypeDef *)port, pin, GPIO_PIN_RESET);
}


void PinOut::ToHi()
{
    HAL_GPIO_WritePin((GPIO_TypeDef *)port, pin, GPIO_PIN_SET);
}
