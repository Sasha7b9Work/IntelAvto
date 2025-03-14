// 2025/02/27 10:37:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/IT6523.h"
#include "LAN/ServerTCP.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "Device/Device.h"
#include "Menu/Pages/Pages.h"
#include "Utils/StringUtils_.h"
#include <cstdarg>
#include <cstdio>


namespace IT6523
{
    static bool is_connected = false;
    static uint time_connect = 0;
    static int pulses_remained = 0;         // �������� ���������
    static bool in_pause = false;

    static TypeSignal::E current = TypeSignal::Count;

    int RemainedPulses()
    {
        return pulses_remained;
    }
}


void IT6523::Update()
{
    // \todo ��� ������ ����� ������, ��� ����� ����� ����������� � ������� �� �� �������� �������
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

    uint period = 5000;

    char buffer[32];

    if (current == TypeSignal::_2b_SAEJ1113)
    {
        IT6523::SendCommandF("carwave:sae:2b:volt %dV", (gset.voltage_mode == VoltageMode::_12) ? 12 : 24);
        IT6523::SendCommandF("carwave:sae:2b:td %s", SU::MillisecondsToSeconds(PageSignal2b::param_td.GetValue().ToInt(), buffer));
        IT6523::SendCommand("carwave:sae:2b:state 1");
    }
    else if (current == TypeSignal::_4_DIN40839)
    {
        IT6523::SendCommandF("carwave:startup:din40839:volt %dV", (gset.voltage_mode == VoltageMode::_12) ? 12 : 24);
        IT6523::SendCommand("carwave:startup:din40839:state 1");
        period = 10000;
    }
    else if (current == TypeSignal::_5a_16750_1)
    {
        IT6523::SendCommand("carwave:iso16750:load:dump:test amode");
        IT6523::SendCommandF("carwave:iso16750:load:dump:volt %dV", (gset.voltage_mode == VoltageMode::_12) ? 12 : 24);
        IT6523::SendCommandF("carwave:iso16750:load:dump:UN %d", 79);
        IT6523::SendCommandF("carwave:iso16750:load:dump:TD %s", "0.04");
        IT6523::SendCommandF("carwave:iso16750:load:dump:state 1");
    }
    else if (current == TypeSignal::_5b_16750_2)
    {
        IT6523::SendCommand("carwave:iso16750:load:dump:test bmode");
        IT6523::SendCommandF("carwave:iso16750:load:dump:volt %dV", (gset.voltage_mode == VoltageMode::_12) ? 12 : 24);
        IT6523::SendCommandF("carwave:iso16750:load:dump:UN %d", PageSignal5b::param_Us.GetValue().ToInt());
        IT6523::SendCommandF("carwave:iso16750:load:dump:US %d", PageSignal5b::param_Us.GetValue().ToInt());
        IT6523::SendCommandF("carwave:iso16750:load:dump:TD %s", SU::MillisecondsToSeconds(PageSignal5b::param_td.GetValue().ToInt(), buffer));
        IT6523::SendCommandF("carwave:iso16750:load:dump:state 1");
    }

    SendCommand("SOURCE:OUTPut:STATE 1");

    Timer::SetPeriodicTask(TimerTask::IT6523, period, IT6523::CallbackOnTimerImpulse);
}


void IT6523::CallbackOnTimerImpulse()
{
    if (!in_pause)
    {
        SendCommand("*trg");

        if (--pulses_remained == 0)
        {
            Device::Stop();
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

    if (current == TypeSignal::_2b_SAEJ1113)
    {
        IT6523::SendCommand("carwave:sae:2b:state 0");
    }
    else if (current == TypeSignal::_4_DIN40839)
    {
        IT6523::SendCommand("carwave:startup:din40839:state 0");
    }
    else if (current == TypeSignal::_5a_16750_1 ||
        current == TypeSignal::_5b_16750_2)
    {
        IT6523::SendCommandF("carwave:iso16750:load:dump:state 0");
    }

    SendCommand("SOURCE:OUTPut:STATE 0");

    Keyboard::AddAction(Key::Stop, Action::Release);
}
