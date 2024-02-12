// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Transceiver.h"
#include "Display/Colors.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"


namespace Transceiver
{
    static void Delay();
}


void Transceiver::Transmit(BaseMessage *message)
{
    while (true)
    {
//        HAL_TIM::DelayMS(50);                                      // ������ �������� 500, ����� ������� ����������
//                                                                    // �������� ������ �������

        HAL_SPI1::CS(true);

        HAL_SPI1::TransmitUInt((uint)message->Size());              // ������� ������ ��������� (4 �����)

        Delay();

        HAL_SPI1::Transmit(message->TakeData(), message->Size());   // ������� ���������

        Delay();

        HAL_SPI1::TransmitUInt(message->CalculateCRC());            // ������� ����������� ����� ���������

        Delay();

        uint recv_crc = HAL_SPI1::ReceiveUInt();

        HAL_SPI1::CS(false);

        if (recv_crc == message->CalculateCRC())
        {
            break;
        }
    }
}


void Transceiver::Delay()
{
    HAL_TIM::DelayUS(5);
}
