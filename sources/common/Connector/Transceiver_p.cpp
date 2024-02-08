// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Transceiver.h"
#include "Display/Colors.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"


void Transceiver::Transmit(BaseMessage *message)
{
    int counter = 0;

    while (true)
    {
        HAL_TIM::DelayMS(500);                                              // ������ �������� 500, ����� ������� ����������
                                                                            // �������� ������ �������
        uint crc = message->CalculateCRC();

        int size = message->Size();

        if (HAL_SPI1::Transmit(size))                                       // ������� ������ ��������� (4 �����)
        {
            if (HAL_SPI1::Transmit(message->TakeData(), message->Size()))   // ������� ���������
            {
                if (HAL_SPI1::Transmit(&crc, sizeof(crc)))                  // ������� ����������� ����� ���������
                {
                    uint code = 0;

                    if (HAL_SPI1::Receive(&code, sizeof(code), 10))         // � ��� ������������� ����� � ������� 10 ��
                    {
                        if (code == crc)
                        {
                            break;
                        }
                    }
                }
            }
        }

        if (counter++ > 10)
        {
            HAL_SPI1::Reset();

            break;
        }
    }
}
