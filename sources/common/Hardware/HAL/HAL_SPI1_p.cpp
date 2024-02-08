// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


namespace HAL_SPI1
{
    namespace CS
    {
        static void ToLow()
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
        }

        static void ToHi()
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
        }

        static void Init()
        {
            GPIO_InitTypeDef is =
            {
                GPIO_PIN_6,
                GPIO_MODE_OUTPUT_PP,
                GPIO_PULLUP,
                GPIO_SPEED_FREQ_VERY_HIGH,
                0
            };

            HAL_GPIO_Init(GPIOB, &is);

            ToHi();
        }
    }

    namespace SCK
    {
        static void ToHi()
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        }

        void ToLow()
        {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        }

        static void Init()
        {
            GPIO_InitTypeDef is =
            {   //  SCK
                GPIO_PIN_5,
                GPIO_MODE_OUTPUT_OD,
                GPIO_PULLUP,
                GPIO_SPEED_FREQ_VERY_HIGH,
                0
            };
            HAL_GPIO_Init(GPIOA, &is);

            ToHi();
        }
    }

    namespace PinOUT
    {
        static void ToHi()
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
        }

        void ToLow()
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
        }

        static void Init()
        {
            GPIO_InitTypeDef is =
            {   //  MO
                GPIO_PIN_5,
                GPIO_MODE_OUTPUT_PP,
                GPIO_PULLUP,
                GPIO_SPEED_FREQ_VERY_HIGH,
                0
            };
            HAL_GPIO_Init(GPIOB, &is);

            ToHi();
        }
    }

    namespace PinIN
    {
        static void Init()
        {
            GPIO_InitTypeDef is =
            {   //  MI
                GPIO_PIN_6,
                GPIO_MODE_INPUT,
                GPIO_PULLUP,
                GPIO_SPEED_FREQ_VERY_HIGH,
                0
            };
            HAL_GPIO_Init(GPIOB, &is);
        }
    }

    static void SendByte(uint8);
}



void HAL_SPI1::Init()
{
    CS::Init();

    SCK::Init();

    PinOUT::Init();

    PinIN::Init();
}


bool HAL_SPI1::Transmit(const void *buffer, int size)
{
    CS::ToLow();

    HAL_TIM::DelayUS(500);

    uint8 *pointer = (uint8 *)buffer;

    for (int i = 0; i < size; i++)
    {
        SendByte(*pointer++);
    }

    HAL_TIM::DelayUS(500);

    CS::ToHi();

    return true;
}


void HAL_SPI1::SendByte(uint8 byte)
{
    for (int i = 0; i < 7; i++)
    {
        SCK::ToLow();

        ((byte &= (1 << i)) != 0) ? PinOUT::ToHi() : PinOUT::ToLow();

        HAL_TIM::DelayUS(100);

        SCK::ToHi();

        HAL_TIM::DelayUS(100);
    }
}


bool HAL_SPI1::Transmit(int value)
{
    return Transmit(&value, 4);
}


bool HAL_SPI1::Receive(void * /*recv*/, int /*size*/, uint /*timeout*/)
{
    CS::ToLow();

    HAL_TIM::DelayUS(500);

//    bool result = HAL_SPI_Receive(&handleSPI1, (uint8 *)recv, (uint16)size, timeout) == HAL_OK;

    HAL_TIM::DelayUS(500);

    CS::ToHi();

    return false;
}
