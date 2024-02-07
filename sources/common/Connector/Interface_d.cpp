// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Messages.h"
#include "Connector/Transceiver.h"
#include "Connector/Handlers_d.h"
#include "Connector/Interface_d.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Queue.h"
#include "Utils/StringUtils.h"
#include <cstdlib>


namespace DInterface
{
    // ������� ���������, ��������� ��������
    static Queue outbox;

    // ��������� �������� ��������, ��� ��� ������� �������� � ������ ��� ���������� ������������ ��������
    uint freqForSend = MAX_UINT;
}


Queue &DInterface::GetOutbox()
{
    return outbox;
}


void DInterface::ResetFreqForSend()
{
    freqForSend = MAX_UINT;
}


void DInterface::Update()
{
    int size = 0;

    if (HAL_SPI1::Receive(&size, 4))                                                            // ����� ������ ������������ ���������
    {
        BaseMessage first;              // ���� ��������� ������ ���������
        BaseMessage second;             // ���� ��������� ������ ���������

        if (first.AllocateMemory(size))
        {
            if (HAL_SPI1::Receive(first.TakeData(), first.Size()))                         // ��������� ������
            {
                if (HAL_SPI1::Transmit(&size, 4))                                          // ������� ��� ������
                {
                    if (HAL_SPI1::Transmit(first.TakeData(), first.Size()))                // � ������
                    {
                        if (HAL_SPI1::Receive(&size, 4))
                        {
                            if (second.AllocateMemory(size))                                        // ������ ��� ��������� ����� ��������� � ���� �����
                            {
                                if (HAL_SPI1::Receive(second.TakeData(), second.Size()))   // ��� � ������
                                {
                                    size = second.Size();

                                    if (HAL_SPI1::Transmit(&size, 4))
                                    {
                                        if (HAL_SPI1::Transmit(second.TakeData(), second.Size()))
                                        {
                                            if (second.IsEquals(&first))                            // ���������, ������� �� ��� �������� ���������
                                            {
                                                DHandlers::Processing(&first);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}


bool DInterface::AddMessageForTransmit(BaseMessage *message)
{
    BaseMessage *clone = message->Clone();

    if (!outbox.Push(clone))
    {
        delete clone;
        // \tood ����� ����� �������� �� ������. ������� ���, ����� ��������� �� ������� �������� ����������������� �������� - �������� �� � ������� ���� ����� ��� ��� ����� (����������)
        // LOG_ERROR("������� �����������");
        return false;
    }

    return true;
}


void BaseMessage::Transmit()
{
    DInterface::AddMessageForTransmit(this);
}
