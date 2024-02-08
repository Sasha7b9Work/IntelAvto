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

    static BaseMessage *CreateMessage(uint8 *data, int size);
}


Queue &DInterface::GetOutbox()
{
    return outbox;
}


void DInterface::Update()
{
    static uint next_time = 0;

    if (TIME_MS < next_time)
    {
        return;
    }

    HAL_SPI1::WaitInterval(400);                                // ∆дЄм нул€ после единицы продолжительностью не менее 400 (на панели сто»т 500)

    int size = 0;

    static const int SIZE_BUFFER = 128;
    uint8 buffer[SIZE_BUFFER];

    if (HAL_SPI1::Receive(&size, 4))
    {
        if (size > SIZE_BUFFER)
        {
            HAL_SPI1::Reset();
            next_time = TIME_MS + 1000;
            return;
        }

        if (HAL_SPI1::Receive(buffer, size))
        {
            uint crc = 0;

            if (HAL_SPI1::Receive(&crc, 4))
            {
                BaseMessage *message = CreateMessage(buffer, size);

                uint new_crc = message ? message->CalculateCRC() : 0;

                HAL_SPI1::Transmit(&new_crc, sizeof(new_crc));

                if (message)
                {
                    if (new_crc == crc)
                    {
                        DHandlers::Processing(*message);
                    }

                    delete message;
                }
            }
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


