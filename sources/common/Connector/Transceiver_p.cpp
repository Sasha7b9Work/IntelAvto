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
        HAL_TIM::DelayMS(500);                                              // ������ �������� 500, ����� ������� ����������
                                                                            // �������� ������ �������
        uint crc = message->CalculateCRC();

        if (HAL_SPI1::Transmit(message->Size()))                            // ������� ������ ��������� (4 �����)
        {
            HAL_TIM::DelayUS(10);

            if (HAL_SPI1::Transmit(message->TakeData(), message->Size()))   // ������� ���������
            {
                HAL_TIM::DelayUS(10);

                if (HAL_SPI1::Transmit(&crc, sizeof(crc)))                  // ������� ����������� ����� ���������
                {
                    HAL_TIM::DelayUS(10);

                    uint8 byte = 0;

                    if (HAL_SPI1::Receive(&byte, 1, 10))                    // � ��� ������������� ����� � ������� 10 ��
                    {
                        break;
                    }
                }
            }
        }
    }
}
