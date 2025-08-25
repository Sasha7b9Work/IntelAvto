// 2025/02/11 09:42:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/RingBuffer.h"
#include "SCPI/Commands.h"
#include "VCP/VCP.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Pages.h"
#include "Utils/StringUtils_.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <cctype>


namespace SCPI
{
    void Send(pchar);
    
    void Error(pchar);

    static RingBuffer ring_buffer;      // Сюда ложим принятые данные прямо в прерывании

    static InBuffer in_buffer;          // А сюда перекладываем из ring_buffer в функции SCPI::Update()

    static void SignalSet(pchar);
    static void SignalGet(pchar);
    static void ModeSet(pchar);
    static void ModeGet(pchar);

    struct StructCommand
    {
        pchar message;
        void (*func)(pchar);
    };

    static const StructCommand commands[] =
    {
        { ":SIGNAL:SET ", SignalSet },
        { ":SIGNAL:GET?", SignalGet },
        { ":MODE:SET ",   ModeSet },
        { ":MODE:GET?",   ModeGet },
        { nullptr, nullptr }
    };

    struct StructPageSignal
    {
        pchar name;
        Page *page;
    };

    static const StructPageSignal pages[] =
    {
        { "1",  PageSignal1::self },
        { "2A", PageSignal2a::self },
        { "2B", PageSignal2b::self },
        { "3A", PageSignal3a::self },
        { "3B", PageSignal3b::self },
        { "4",  PageSignal4::self },
        { "5A", PageSignal5a::self },
        { "5B", PageSignal5b::self },
        { "",   nullptr }
    };
}


void SCPI::AppendNewData(uint8 *buffer, uint size)
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

    const StructCommand *command = &commands[0];

    while (command->message)
    {
        uint num_symbols = std::strlen(command->message);

        if (std::memcmp((void *)symbols, (void *)command->message, num_symbols) == 0)
        {
            command->func(symbols + num_symbols);
            return new CommandNull();
        }

        command++;
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


void SCPI::SignalSet(pchar params)
{
    const StructPageSignal *chan = &pages[0];

    while (chan->page)
    {
        if (SU::EqualsZeroStrings(chan->name, params))
        {
            PageMain::SetPage(chan->page);
            return;
        }

        chan++;
    }

    SCPI::Error(params);
}


void SCPI::SignalGet(pchar)
{
    const StructPageSignal *chan = &pages[0];

    while (chan->page)
    {
        if (chan->page == Menu::OpenedPage())
        {
            SCPI::Send(chan->name);
            return;
        }

        chan++;
    }
}


void SCPI::ModeSet(pchar params)
{
    if (SU::EqualsZeroStrings(params, "12V"))
    {
        gset.voltage_mode.current = VoltageMode::_12;
    }
    else if (SU::EqualsZeroStrings(params, "24V"))
    {
        gset.voltage_mode.current = VoltageMode::_24;
    }
    else
    {
        Error(params);
    }
}


void SCPI::ModeGet(pchar params)
{
    if (params[0])
    {
        Error(params);
    }
    else
    {
        SCPI::Send(VoltageMode::Is12() ? "12V" : "24V");
    }
}
