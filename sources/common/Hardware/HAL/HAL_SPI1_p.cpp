// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


namespace HAL_SPI1
{
    // Для связи с основным процессором
    static SPI_HandleTypeDef handleSPI1 =
    {
        SPI1,
        {
            SPI_MODE_MASTER,
            SPI_DIRECTION_2LINES,
            SPI_DATASIZE_8BIT,
            SPI_POLARITY_HIGH,
            SPI_PHASE_2EDGE,
            SPI_NSS_HARD_OUTPUT,
            SPI_BAUDRATEPRESCALER_32,
            SPI_FIRSTBIT_MSB,
            SPI_TIMODE_DISABLED,
            SPI_CRCCALCULATION_DISABLED,
            7
        },
        nullptr, 0, 0, nullptr, 0, 0, nullptr, nullptr, nullptr, nullptr, HAL_UNLOCKED, HAL_SPI_STATE_RESET, 0
    };

    // Возвращает true, если прибор готов к обмену
    static bool IsReady();

    namespace CS
    {
        static void ToLow()
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
        }
        static void Init()
        {
            GPIO_InitTypeDef is =
            {
                GPIO_PIN_6,
                GPIO_MODE_AF_PP,
                GPIO_PULLUP,
                GPIO_SPEED_HIGH,
                GPIO_AF5_SPI1
            };

            HAL_GPIO_Init(GPIOB, &is);

            ToLow();
        }
    }
}



void HAL_SPI1::Init()
{
    CS::Init();

    GPIO_InitTypeDef is =
    {   //  SCK         MI
        GPIO_PIN_5 | GPIO_PIN_6,
        GPIO_MODE_AF_PP,
        GPIO_PULLUP,
        GPIO_SPEED_HIGH,
        GPIO_AF5_SPI1
    };
    HAL_GPIO_Init(GPIOA, &is);

    is.Pin = GPIO_PIN_5;            // MO
    HAL_GPIO_Init(GPIOB, &is);

    HAL_SPI_Init(&handleSPI1);
}


bool HAL_SPI1::Transmit(const void *buffer, int size)
{
    if (HAL_SPI_Transmit(&handleSPI1, (uint8 *)((void *)buffer), (uint16)size, 100) != HAL_OK)
    {
        return false;
    }

    return true;
}


bool HAL_SPI1::Transmit(int value)
{
    return Transmit(&value, 4);
}


bool HAL_SPI1::Receive(void *recv, int size)
{
    if (HAL_SPI_Receive(&handleSPI1, (uint8 *)recv, (uint16)size, 100) != HAL_OK)
    {
        return false;
    }

    return true;
}


uint HAL_SPI1::ReceiveAndCompare(const void *compared, int size)
{
    uint result = 0;

    uint8 byte = 0;

    uint8 *data = (uint8 *)((void *)compared);

    for (int i = 0; i < size; i++)
    {
        if (Receive(&byte, 1) && data[i] != byte)
        {
            result++;
        }
    }

    return result;
}


bool HAL_SPI1::WaitRelease()
{
    TimeMeterMS meter;

    while (IsReady())
    {
        if (meter.ElapsedTime() > 100)
        {
            return false;
        }

    }   // Если попали в время сигнала готовности, пропустим его, чтобы транзакция гарантированно начиналась после разрешающего фронта

    while (!IsReady())
    {
        if (meter.ElapsedTime() > 100)
        {
            return false;
        }
    }  // Теперь ожидаем, когда придёт сигнал готовности

    return true;
}


bool HAL_SPI1::IsReady()
{
    return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_SET;
}
