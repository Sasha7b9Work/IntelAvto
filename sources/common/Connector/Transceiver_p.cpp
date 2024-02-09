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
        HAL_TIM::DelayMS(500);                                      // ������ �������� 500, ����� ������� ����������
                                                                    // �������� ������ �������
        HAL_SPI1::TransmitUInt((uint)message->Size());              // ������� ������ ��������� (4 �����)

        HAL_SPI1::Transmit(message->TakeData(), message->Size());   // ������� ���������

        HAL_SPI1::TransmitUInt(message->CalculateCRC());            // ������� ����������� ����� ���������

        if (HAL_SPI1::ReceiveUInt() == message->CalculateCRC())
        {
            break;
        }
    }
}
