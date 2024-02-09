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

    static BaseMessage *CreateMessage(uint8 *data, int size);
}


Queue &DInterface::GetOutbox()
{
    return outbox;
}


void DInterface::Update()
{
    static const int SIZE_BUFFER = 128;
    uint8 buffer[SIZE_BUFFER];

    HAL_SPI1::WaitCS();

    int size = (int)HAL_SPI1::ReceiveUInt();

    if (size < SIZE_BUFFER)
    {
        HAL_SPI1::Receive(buffer, size);

        uint crc = HAL_SPI1::ReceiveUInt();

        BaseMessage *message = CreateMessage(buffer, size);

        uint value = message ? message->CalculateCRC() : 0;
        
        HAL_SPI1::TransmitUInt(value);

        if (message)
        {
            if (message->CalculateCRC() == crc)
            {
                DHandlers::Processing(*message);
            }

            delete message;
        }
    }
}


BaseMessage *DInterface::CreateMessage(uint8 *data, int size)
{
    if (size >= 8)
    {
        uint *pointer = (uint *)data;

        pointer++;

        Command::E command = (Command::E)*pointer++;

        if (command == Command::STOP)
        {
            return new Message::Stop();
        }
        else if (command == Command::START_2A)
        {
            Value Us((int)(*pointer++));
            Value t1((int)(*pointer++));

            return new Message::Start2A(Us, t1);
        }
    }

    return nullptr;
}


void BaseMessage::Transmit()
{
}


