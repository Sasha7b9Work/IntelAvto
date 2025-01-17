// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Transceiver_.h"
#include "Display/Colors.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"


namespace Transceiver
{
    void Delay();
}


void Transceiver::Transmit(BaseMessage *message)
{
    (void)message;

#ifndef GUI
    while (true)
    {
        HAL_SPI1::CS(true);

        HAL_SPI1::TransmitUInt((uint)message->Size());              // Передаём размер сообщения (4 байта)

        Delay();

        HAL_SPI1::Transmit(message->TakeData(), message->Size());   // Передаём сообщение

        Delay();

        HAL_SPI1::TransmitUInt(message->CalculateCRC());            // Передаём контрольную сумму сообщения

        Delay();

        uint recv_crc = HAL_SPI1::ReceiveUInt();

        HAL_SPI1::CS(false);

        if (recv_crc == message->CalculateCRC())
        {
            break;
        }
    }
#endif
}


void Transceiver::Delay()
{
    HAL_TIM::DelayUS(4);
}
