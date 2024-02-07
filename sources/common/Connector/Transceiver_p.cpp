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
        HAL_SPI1::Transmit(message->Size());                            // Передаём размер сообщения (4 байта)

        uint crc = message->CalculateCRC();

        HAL_SPI1::Transmit(message->TakeData(), message->Size());       // Передаём сообщение

        HAL_SPI1::Transmit(&crc, sizeof(crc));                          // Передаём контрольную сумму сообщения

        uint8 byte = 0;

        if (HAL_SPI1::Receive(&byte, 1, 10))                            // И ждём подтверждения приёма в течение 10 мс
        {
            break;
        }
    }
}
