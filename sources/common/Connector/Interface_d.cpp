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
    // ќчередь сообщений, ожидающих отправки
    static Queue outbox;

    // Ќенулевое значение означает, что его следует передать в панель как измеренное частотомером значение
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
        // \tood «десь нужно сообщать об ошибке. —делать так, чтобы сообщени€ об ошибках обладали привилегированным статусом - помещать их в очередь даже когда там нет места (освободить)
        // LOG_ERROR("ќчередь переполнена");
        return false;
    }

    return true;
}


void BaseMessage::Transmit()
{
    DInterface::AddMessageForTransmit(this);
}
