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
    uint size = 0;

    if (HAL_SPI1::Receive(&size, 4))
    {
        size = size;
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
