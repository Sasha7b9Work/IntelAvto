// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Generator/Generator.h"
#include "Generator/MAX532.h"
#include "Generator/MCP4811.h"
#include "Connector/Device/Interface_d.h"
#include "Connector/Device/Messages_.h"
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
                message->ResetPointer();

                Command::E command = message->PopCommand();

                if (command == Command::STOP)
                {
                    Generator::Stop();
                }
                else if (command == Command::PAUSE)
                {
                    Generator::Pause();
                }
                else if (command == Command::RESUME)
                {
                    Generator::Resume();
                }
                else if (command == Command::START_1_12V)
                {
                    Value Us = message->PopValue();
                    Value t1 = message->PopValue();

                    Generator::Start1_12V(Us, t1);
                }
                else if (command == Command::START_1_24V)
                {
                    Value Us = message->PopValue();
                    Value t1 = message->PopValue();

                    Generator::Start1_24V(Us, t1);
                }
                else if (command == Command::START_2A)
                {
                    Value Us = message->PopValue();
                    Value t1 = message->PopValue();

                    Generator::Start2A(Us, t1);
                }
                else if (command == Command::START_3A)
                {
                    Value Us = message->PopValue();

                    Generator::Start3A(Us);
                }
                else if (command == Command::START_3B)
                {
                    Value Us = message->PopValue();

                    Generator::Start3B(Us);
                }
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
        else if (command == Command::PAUSE)
        {
            return new Message::Pause();
        }
        else if (command == Command::RESUME)
        {
            return new Message::Resume();
        }
        else if (command == Command::START_1_12V)
        {
            Value Us = Value((uint)(*pointer++));       // Амплитуда
            Value t1 = Value((uint)(*pointer++));       // Период повторения

            return new Message::Start1_12V(Us, t1);
        }
        else if (command == Command::START_1_24V)
        {
            Value Us = Value((uint)(*pointer++));
            Value t1 = Value((uint)(*pointer++));

            return new Message::Start1_24V(Us, t1);
        }
        else if (command == Command::START_2A)
        {
            Value Us((uint)(*pointer++));
            Value t1((uint)(*pointer++));

            return new Message::Start2A(Us, t1);
        }
        else if (command == Command::START_3A)
        {
            Value Us((uint)(*pointer++));

            return new Message::Start3A(Us);
        }
        else if (command == Command::START_3B)
        {
            Value Us((uint)(*pointer++));

            return new Message::Start3B(Us);
        }
    }

    return nullptr;
}


void BaseMessage::Transmit()
{
}


