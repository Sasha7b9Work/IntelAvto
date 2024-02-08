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
}


void HAL_SPI1::Init()
{
    pinCS.Init();
    pinOUT.Init();
    pinIN.Init();
    pinSCK.Init();
}


bool HAL_SPI1::Receive(void *buffer, int size)
{
    while (pinCS.IsHi())
    {
    }

    uint8 *pointer = (uint8 *)buffer;

    for (int i = 0; i < size; i++)
    {
        *pointer++ = ReceiveByte();
    }

    return true;
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


bool HAL_SPI1::Transmit(void * /*buffer*/, int /*size*/)
{
    return false;
//    return HAL_SPI_Transmit(&handle, (uint8 *)buffer, (uint16)size, 100) == HAL_OK;
}


bool HAL_SPI1::Transmit(int value)
{
    return Transmit(&value, 4);
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
