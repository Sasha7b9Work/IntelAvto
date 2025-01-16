// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"
#include <stm32f4xx_hal.h>


namespace HAL_SPI1
{                                               // DD4 Device XP4 | Panel XP3    DD1
    static PinIn pinCS(GPIOA, GPIO_PIN_2);      // 36     5       |    5      92    PB6
    static PinIn pinIN(GPIOA, GPIO_PIN_7);      // 43     3       |    3      91    PB5  MOSI
    static PinIn pinSCK(GPIOA, GPIO_PIN_5);     // 41     1       |    1      30    PA5
    static PinOut pinOUT(GPIOA, GPIO_PIN_6);    // 42     7       |    7      31    PA6  MISO

    static uint8 ReceiveByte();
    static void SendByte(uint8);
}


void HAL_SPI1::Init()
{
    pinCS.Init();
    pinOUT.Init();
    pinIN.Init();
    pinSCK.Init();
}


void HAL_SPI1::Receive(void *buffer, int size)
{
    uint8 *pointer = (uint8 *)buffer;

    for (int i = 0; i < size; i++)
    {
        *pointer++ = ReceiveByte();
    }
}


uint8 HAL_SPI1::ReceiveByte()
{
    uint8 result = 0;

    for (int i = 0; i < 8; i++)
    {
        while (pinSCK.IsLow())
        {
        }

        if (pinIN.IsHi())
        {
            result |= (1 << i);
        }

        while (pinSCK.IsHi())
        {
        }
    }

    return result;
}


void HAL_SPI1::SendByte(uint8 byte)
{
    for (int i = 0; i < 8; i++)
    {
        ((byte & (1 << i)) == 0) ? pinOUT.ToLow() : pinOUT.ToHi();

        while (pinSCK.IsLow())
        {
        }

        while (pinSCK.IsHi())
        {
        }
    }
}


void HAL_SPI1::Transmit(void *buffer, int size)
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


uint HAL_SPI1::ReceiveUInt()
{
    uint value = 0;

    Receive(&value, sizeof(value));

    return value;
}


void HAL_SPI1::WaitCS()
{
    while (pinCS.IsLow())
    {
    }
    while (pinCS.IsHi())
    {
    }
}
