// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


/*

    +-------+----------+----------+
    | Вывод |  Панель  |  Device  |
    +-------+----------+----------+
    | CS    | PB6 O 92 | PA2 I 36 |
    | SCK   | PA5 O 30 | PA5 I 41 |
    | MOSI  | PB5 O 91 | PA7 I 43 |
    | MISO  | PA6 I 31 | PA6 O 42 |
    +-------+----------+----------+

*/


namespace HAL_SPI1
{
    static PinOut pinCS(GPIOB, GPIO_PIN_6);
    static PinOut pinSCK(GPIOA, GPIO_PIN_5);
    static PinOut pinOUT(GPIOB, GPIO_PIN_5);
    static PinIn  pinIN(GPIOA, GPIO_PIN_6);

    static void SendByte(uint8);

    static uint8 ReceiveByte();

    static void Receive(void *recv, int size);

    static void Delay()
    {
        HAL_TIM::DelayUS(5);
    }
}


void HAL_SPI1::CS(bool cs)
{
    cs ? pinCS.ToLow() : pinCS.ToHi();
}


void HAL_SPI1::Init()
{
    pinCS.Init();
    pinCS.ToHi();

    pinSCK.Init();
    pinSCK.ToLow();

    pinOUT.Init();

    pinIN.Init();
}


void HAL_SPI1::Transmit(const void *buffer, int size)
{
    uint8 *pointer = (uint8 *)buffer;

    for (int i = 0; i < size; i++)
    {
        SendByte(*pointer++);
    }
}


void HAL_SPI1::TransmitUInt(uint value)
{
    Transmit(&value, 4);
}


void HAL_SPI1::Receive(void *recv, int size)
{
    uint8 *pointer = (uint8 *)recv;

    for (int i = 0; i < size; i++)
    {
        *pointer++ = ReceiveByte();
    }
}


void HAL_SPI1::SendByte(uint8 byte)
{
    for (int i = 0; i < 8; i++)
    {
        ((byte & (1 << i)) == 0) ? pinOUT.ToLow() : pinOUT.ToHi();

        Delay();

        pinSCK.ToHi();

        Delay();

        pinSCK.ToLow();
    }
}


uint8 HAL_SPI1::ReceiveByte()
{
    uint8 byte = 0;

    for (int i = 0; i < 8; i++)
    {
        Delay();

        if (pinIN.IsHi())
        {
            byte |= (1 << i);
        }

        pinSCK.ToHi();

        Delay();

        pinSCK.ToLow();
    }

    return byte;
}


uint HAL_SPI1::ReceiveUInt()
{
    uint value = 0;

    HAL_SPI1::Receive(&value, sizeof(value));

    return value;
}
