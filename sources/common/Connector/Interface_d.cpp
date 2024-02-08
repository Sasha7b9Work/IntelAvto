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
    // Очередь сообщений, ожидающих отправки
    static Queue outbox;

    // Ненулевое значение означает, что его следует передать в панель как измеренное частотомером значение
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
    HAL_SPI1::WaitInterval(400);                                // Ждём нуля после единицы продолжительностью не менее 400 (на панели стоИт 500)

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
        // \tood Здесь нужно сообщать об ошибке. Сделать так, чтобы сообщения об ошибках обладали привилегированным статусом - помещать их в очередь даже когда там нет места (освободить)
        // LOG_ERROR("Очередь переполнена");
        return false;
    }

    return true;
}


void BaseMessage::Transmit()
{
    DInterface::AddMessageForTransmit(this);
}
