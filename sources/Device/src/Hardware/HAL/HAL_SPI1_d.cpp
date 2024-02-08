// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


namespace HAL_SPI1
{
    namespace CS
    {
        void Init()
        {
            GPIO_InitTypeDef is =
            {   //  NSS
                GPIO_PIN_2,
                GPIO_MODE_INPUT,
                GPIO_PULLUP
            };
            HAL_GPIO_Init(GPIOA, &is);
        }

        bool IsLow()
        {
            return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == GPIO_PIN_RESET;
        }

        // Вывод находится в "активном" состоянии - панель что-то передаёт.
        bool IsActive()
        {
            return IsLow();
        }
    }

    namespace PinIN
    {
        void Init()
        {
            GPIO_InitTypeDef is =
            {   //  MO
                GPIO_PIN_7,
                GPIO_MODE_INPUT,
                GPIO_PULLUP
            };
            HAL_GPIO_Init(GPIOA, &is);
        }

        bool IsHi()
        {
            return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7) != GPIO_PIN_RESET;
        }
    }

    namespace SCK
    {
        static void Init()
        {
            GPIO_InitTypeDef is =
            {   //  SCK
                GPIO_PIN_5,
                GPIO_MODE_INPUT,
                GPIO_PULLUP
            };
            HAL_GPIO_Init(GPIOA, &is);
        }

        static bool IsLow()
        {
            return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET;
        }

        static bool IsHi()
        {
            return !IsLow();
        }
    }

    namespace PinOUT
    {
        void Init()
        {
            GPIO_InitTypeDef isGPIOA =
            {   //  MI
                GPIO_PIN_6,
                GPIO_MODE_OUTPUT_OD,
                GPIO_PULLUP
            };
            HAL_GPIO_Init(GPIOA, &isGPIOA);
        }
    }

    static uint8 ReceiveByte();
}


void HAL_SPI1::Init()
{
    CS::Init();
    PinOUT::Init();
    PinIN::Init();
    SCK::Init();
}


bool HAL_SPI1::Receive(void *buffer, int size)
{
    while (!CS::IsActive())
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

    for (int i = 7; i >= 0; i--)
    {
        while (SCK::IsLow())
        {
        }

        if (PinIN::IsHi())
        {
            result |= (1 << i);
        }

        while (SCK::IsHi())
        {
        }

        if (i != 7)
        {
            result <<= 1;
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
        while (CS::IsLow())  { }

        uint time_start = TIME_MS;

        while (!CS::IsLow()) { }

        if (TIME_MS - time_start >= timeMS)
        {
            return;
        }
    }
}
