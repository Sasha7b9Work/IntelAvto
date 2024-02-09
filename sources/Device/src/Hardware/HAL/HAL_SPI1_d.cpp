// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


namespace HAL_SPI1
{
    static PinIn pinCS(GPIOA, GPIO_PIN_2);
    static PinIn pinIN(GPIOA, GPIO_PIN_7);
    static PinIn pinSCK(GPIOA, GPIO_PIN_5);
    static PinOut pinOUT(GPIOA, GPIO_PIN_6);

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
    while (pinCS.IsHi())
    {
    }

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

        HAL_TIM::DelayUS(50);

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

    while (pinCS.IsHi())
    {
    }

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


void HAL_SPI1::WaitInterval(uint timeMS)
{
    while (true)
    {
        while (pinCS.IsLow())  { }

        uint time_start = TIME_MS;

        while (pinCS.IsHi()) { }

        if (TIME_MS - time_start >= timeMS)
        {
            return;
        }
    }
}
