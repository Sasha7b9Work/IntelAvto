// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


/*

    +-------+---------+--------+
    | Вывод |  Панель | Device |
    +-------+---------+--------+
    | CS    | PB6 O   | PA2 I  |
    | SCK   | PA5 O   | PA5 I  |
    | MOSI  | PB5 O   | PA7 I  |
    | MISO  | PA6 I   | PA6 O  |
    +-------+---------+--------+

*/


namespace HAL_SPI1
{
    static PinOut pinCS(GPIOD, GPIO_PIN_6);
    static PinOut pinSCK(GPIOA, GPIO_PIN_5);
    static PinOut pinOUT(GPIOB, GPIO_PIN_5);
    static PinIn  pinIN(GPIOA, GPIO_PIN_6);

    static void SendByte(uint8);

    static uint8 ReceiveByte();
}



void HAL_SPI1::Init()
{
    pinCS.Init();

    pinSCK.Init();
    pinSCK.ToHi();

    pinOUT.Init();

    pinIN.Init();
}


bool HAL_SPI1::Transmit(const void *buffer, int size)
{
    pinCS.ToLow();

    HAL_TIM::DelayUS(500);

    uint8 *pointer = (uint8 *)buffer;

    for (int i = 0; i < size; i++)
    {
        SendByte(*pointer++);
    }

    HAL_TIM::DelayUS(500);

    pinCS.ToHi();

    return true;
}


bool HAL_SPI1::Transmit(int value)
{
    return Transmit(&value, 4);
}


bool HAL_SPI1::Receive(void *recv, int size)
{
    pinSCK.ToLow();
    pinCS.ToLow();

    HAL_TIM::DelayUS(500);

    uint8 *pointer = (uint8 *)recv;

    for (int i = 0; i < size; i++)
    {
        *pointer++ = ReceiveByte();
    }

    HAL_TIM::DelayUS(500);

    pinCS.ToHi();

    return false;
}


void HAL_SPI1::SendByte(uint8 byte)
{
    for (int i = 0; i < 8; i++)
    {
        pinSCK.ToLow();

        ((byte &= (1 << i)) != 0) ? pinOUT.ToHi() : pinOUT.ToLow();

        HAL_TIM::DelayUS(100);

        pinSCK.ToHi();

        HAL_TIM::DelayUS(100);
    }
}


uint8 HAL_SPI1::ReceiveByte()
{
    uint8 byte = 0;

    for (int i = 0; i < 8; i++)
    {
        pinSCK.ToLow();

        HAL_TIM::DelayUS(100);

        if (pinIN.IsHi())
        {
            byte |= (1 << i);
        }

        pinSCK.ToHi();

        HAL_TIM::DelayUS(100);
    }

    return byte;
}
