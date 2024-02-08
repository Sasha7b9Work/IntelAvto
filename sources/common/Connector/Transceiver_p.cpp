// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Transceiver.h"
#include "Display/Colors.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"


void Transceiver::Transmit(BaseMessage *message)
{
    while (true)
    {
        HAL_TIM::DelayMS(500);                                              // Делаем интервал 500, чтобы приёмное устройство
                                                                            // отловило начало посылки
        uint crc = message->CalculateCRC();

        if (HAL_SPI1::Transmit(message->Size()))                            // Передаём размер сообщения (4 байта)
        {
            HAL_TIM::DelayUS(10);

            if (HAL_SPI1::Transmit(message->TakeData(), message->Size()))   // Передаём сообщение
            {
                HAL_TIM::DelayUS(10);

                if (HAL_SPI1::Transmit(&crc, sizeof(crc)))                  // Передаём контрольную сумму сообщения
                {
                    HAL_TIM::DelayUS(10);

                    uint8 byte = 0;

                    if (HAL_SPI1::Receive(&byte, 1, 10))                    // И ждём подтверждения приёма в течение 10 мс
                    {
                        break;
                    }
                }
            }
        }
    }
}
