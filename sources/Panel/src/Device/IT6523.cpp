// 2025/02/27 10:37:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/IT6523.h"
#include "LAN/ServerTCP.h"
#include "Hardware/Timer.h"
#include <cstdarg>
#include <cstdio>


namespace IT6523
{
    static bool is_connected = false;
    static uint time_connect = 0;
}


void IT6523::Update()
{
    // \todo Эти пляски нужны потому, что сразу после подключения к серверу он не получает команду
    {
        if (!ServerTCP::IsConnected())
        {
            is_connected = false;

            time_connect = 0;

            return;
        }

        if (time_connect == 0)
        {
            time_connect = TIME_MS;
        }

        if (!is_connected)
        {
            if (TIME_MS - time_connect < 5000)
            {
                return;
            }

            is_connected = true;

            SendCommand("SYSTem:REMote");
        }
    }
}


void IT6523::SendCommand(pchar message)
{
    if (message[0] != '*')
    {
        ServerTCP::SendString(":");
    }

    ServerTCP::SendString(message);

    ServerTCP::SendString("\x0d\x0a");
}


void IT6523::SendCommandF(pchar format, ...)
{
    static char buffer[128];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    SendCommand(buffer);
}


void IT6523::Start()
{
    SendCommand("SOURCE:OUTPut:STATE 1");
    SendCommand("*TRG");
}


void IT6523::Pause()
{

}


void IT6523::Resume()
{

}


void IT6523::Stop()
{
    SendCommand("SOURCE:OUTPut:STATE 0");
}
