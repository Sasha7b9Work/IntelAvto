// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


namespace HAL_SPI1
{

    static SPI_HandleTypeDef handle =                                   // Для связи с панелью
    {
        SPI1,
        {
            SPI_MODE_SLAVE,                 // Init.Mode
            SPI_DIRECTION_2LINES,           // Init.Direction
            SPI_DATASIZE_8BIT,              // Init.DataSize
            SPI_POLARITY_HIGH,              // Init.CLKPolarity
            SPI_PHASE_2EDGE,                // Init.CLKPhase
            SPI_NSS_SOFT,                   // Init.NSS
            SPI_BAUDRATEPRESCALER_32,       // Init.BaudRatePrescaler
            SPI_FIRSTBIT_MSB,               // Init.FirstBit
            SPI_TIMODE_DISABLED,            // Init.TIMode
            SPI_CRCCALCULATION_DISABLED,    // Init.CRCCalculation
            7                               // InitCRCPolynomial
        },
        nullptr, 0, 0, nullptr, 0, 0, nullptr, nullptr, nullptr, nullptr, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
    };

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

            HAL_SPI_Init(&handle);
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
}


void HAL_SPI1::Init()
{
    CS::Init();

    GPIO_InitTypeDef isGPIOA =
    {   //  SCK         MI           MO
        GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_PULLUP,
        GPIO_SPEED_HIGH,
        GPIO_AF5_SPI1
    };
    HAL_GPIO_Init(GPIOA, &isGPIOA);

    HAL_SPI_Init(&handle);
}


bool HAL_SPI1::Receive(void *buffer, int size)
{
    while (!CS::IsActive())
    {
    }

    return HAL_SPI_Receive(&handle, (uint8 *)buffer, (uint16)size, 100) == HAL_OK;
}


bool HAL_SPI1::Transmit(void *buffer, int size)
{
    return HAL_SPI_Transmit(&handle, (uint8 *)buffer, (uint16)size, 100) == HAL_OK;
}


bool HAL_SPI1::Transmit(int value)
{
    return Transmit(&value, 4);
}
