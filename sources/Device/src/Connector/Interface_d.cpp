// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Messages_.h"
#include "Connector/Transceiver_.h"
#include "Connector/Interface_d.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Queue.h"
#include "Utils/StringUtils.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Generator/Generator.h"
#include "Generator/MAX532.h"
#include "Generator/MCP4811.h"
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
                else if (command == Command::START_1)
                {
                    Value Us = message->PopValue();
                    Value period = message->PopValue();
                    Value duration = message->PopValue();

                    Generator::Start1(Us, period, duration);
                }
                else if (command == Command::START_2A)
                {
                    Value Us = message->PopValue();
                    Value period = message->PopValue();
                    Value duration = message->PopValue();

                    Generator::Start2A(Us, period, duration);
                }
                else if (command == Command::START_3A)
                {
                    Value Us = message->PopValue();
                    Value duration = message->PopValue();

                    Generator::Start3A(Us, duration);
                }
                else if (command == Command::START_3B)
                {
                    Value Us = message->PopValue();
                    Value duration = message->PopValue();

                    Generator::Start3B(Us, duration);
                }
                else if (command == Command::SET_VOLTAGE)
                {
                    Value U = message->PopValue();

                    MAX532::SetVoltage(U);
                    MCP4811::SetVoltage(U);
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
        else if (command == Command::START_1)
        {
            Value Us((uint)(*pointer++));
            Value period((uint)(*pointer++));
            Value duration((uint)(*pointer++));

            return new Message::Start1(Us, period, duration);
        }
        else if (command == Command::START_2A)
        {
            Value Us((uint)(*pointer++));
            Value t1((uint)(*pointer++));
            Value td((uint)(*pointer++));

            return new Message::Start2A(Us, t1, td);
        }
        else if (command == Command::START_3A)
        {
            Value Us((uint)(*pointer++));
            Value duration((uint)(*pointer++));

            return new Message::Start3A(Us, duration);
        }
        else if (command == Command::START_3B)
        {
            Value Us((uint)(*pointer++));
            Value duration((uint)(*pointer++));

            return new Message::Start3B(Us, duration);
        }
        else if (command == Command::SET_VOLTAGE)
        {
            Value U((uint)(*pointer++));

            return new Message::SetVoltage(U);
        }
    }

    return nullptr;
}


void BaseMessage::Transmit()
{
}


