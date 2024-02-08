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

        int size = message->Size();

        if (HAL_SPI1::Transmit(size))                                       // Передаём размер сообщения (4 байта)
        {
            if (HAL_SPI1::Transmit(message->TakeData(), message->Size()))   // Передаём сообщение
            {
                if (HAL_SPI1::Transmit(&crc, sizeof(crc)))                  // Передаём контрольную сумму сообщения
                {
                    uint code = 0;

                    if (HAL_SPI1::Receive(&code, sizeof(code), 10))         // И ждём подтверждения приёма в течение 10 мс
                    {
                        if (code == crc)
                        {
                            break;
                        }
                        else
                        {
                            HAL_SPI1::Reset();
                        }
                    }
                    else
                    {
                        HAL_SPI1::Reset();
                    }
                }
            }
        }
        
//        if(counter++ > 10)
//        {
//            HAL_SPI1::Reset();
//            break;
//        }
    }
}
