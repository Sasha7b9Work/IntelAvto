// 2025/02/11 09:42:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/RingBuffer.h"
#include "SCPI/Commands.h"
#include "VCP/VCP.h"
#include <cctype>


namespace SCPI
{
    void Send(pchar);
    
    void Error(pchar);

    static RingBuffer ring_buffer;      // Сюда ложим принятые данные прямо в прерывании

    static InBuffer in_buffer;          // А сюда перекладываем из ring_buffer в функции SCPI::Update()
}


void SCPI::AddNewData(uint8 *buffer, uint size)
{
    for (uint i = 0; i < size; i++)
    {
        char symbol = (char)*buffer;
        buffer++;

        ring_buffer.Append((uint8)std::toupper(symbol));
    }
}


void SCPI::Update()
{
    ring_buffer.GetData(in_buffer);
    in_buffer.Update();
}


void SCPI::InBuffer::Update()
{
    bool run = true;

    while (run)
    {
        Command *command = ExtractCommand();

        run = command->Execute();

        delete command;
    }
}


SCPI::Command *SCPI::InBuffer::ExtractCommand()
{
    while (Size() && (buffer[0] == 0x0d || buffer[0] == 0x0a))
    {
        RemoveFirst(1);
    }

    for (int i = 0; i < Size(); i++)
    {
        if (buffer[i] == 0x0d || buffer[i] == 0x0a)
        {
            if (i == 0)
            {
                break;
            }
            else
            {
                char symbols[64];

                std::memcpy(symbols, buffer, (uint)i);
                symbols[i] = '\0';

                RemoveFirst(i);

                return ParseCommand(symbols);
            }
        }
    }

    return new CommandNull();
}


SCPI::Command *SCPI::InBuffer::ParseCommand(pchar symbols)
{
    char *data = (char *)symbols;

    if (std::strcmp(data, "*IDN?") == 0)                // *IDN?
    {
        return new CommandIDN();
    }

    SCPI::Error(symbols);

    return new CommandNull();
}


void SCPI::Send(pchar message)
{
    VCP::SendStringAsynch0D0A((char *)message);
}


void SCPI::Error(pchar text)
{
    VCP::SendStringAsynchRAW((char *)"ERROR !!! Unknown sequence : ");
    VCP::SendStringAsynch0D0A((char *)text);
}
