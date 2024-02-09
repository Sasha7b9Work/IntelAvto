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
        HAL_TIM::DelayMS(500);                                      // Делаем интервал 500, чтобы приёмное устройство
                                                                    // отловило начало посылки
        HAL_SPI1::TransmitUInt((uint)message->Size());              // Передаём размер сообщения (4 байта)

        HAL_SPI1::Transmit(message->TakeData(), message->Size());   // Передаём сообщение

        HAL_SPI1::TransmitUInt(message->CalculateCRC());            // Передаём контрольную сумму сообщения

        if (HAL_SPI1::ReceiveUInt() == message->CalculateCRC())
        {
            break;
        }
    }
}
