// 2025/02/27 10:37:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/IT6523.h"
#include "LAN/ServerTCP.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include <cstdarg>
#include <cstdio>


namespace IT6523
{
    static bool is_connected = false;
    static uint time_connect = 0;
    static int pulses_remained = 0;         // Осталось импульсов
    static bool in_pause = false;

    static TypeSignal::E current = TypeSignal::Count;
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
            SendCommand("trigger:source bus");
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


void IT6523::Start(TypeSignal::E type, int num_pulses)
{
    current = type;

    pulses_remained = num_pulses;

    if (current == TypeSignal::_2b)
    {
        IT6523::SendCommand("carwave:sae:2b:volt 12V");
        IT6523::SendCommand("carwave:sae:2b:td 0.2");
        IT6523::SendCommand("carwave:sae:2b:state 1");
    }
    else if (current == TypeSignal::_4)
    {

    }
    else if (current == TypeSignal::_5a)
    {

    }
    else if (current == TypeSignal::_5b)
    {

    }

    SendCommand("SOURCE:OUTPut:STATE 1");

    Timer::SetPeriodicTask(TimerTask::IT6523, 2000, IT6523::CallbackOnTimerImpulse);
}


void IT6523::CallbackOnTimerImpulse()
{
    if (!in_pause)
    {
        SendCommand("*trg");

        if (--pulses_remained == 0)
        {
            Stop();
        }
    }
}


void IT6523::Pause()
{
    in_pause = true;
}


void IT6523::Resume()
{
    in_pause = false;
}


void IT6523::Stop()
{
    Timer::DisableTask(TimerTask::IT6523);

    if (current == TypeSignal::_2b)
    {
        IT6523::SendCommand("carwave:sae:2b:state 0");
    }

    SendCommand("SOURCE:OUTPut:STATE 0");

    Keyboard::AddAction(Key::Stop, Action::Release);
}
