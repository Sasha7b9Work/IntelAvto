// 2025/02/27 10:37:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/IT6523.h"
#include "LAN/ServerTCP.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "Device/Device.h"
#include "Utils/StringUtils_.h"
#include <cstdarg>
#include <cstdio>


namespace IT6523
{
    static bool is_connected = false;
    static uint time_connect = 0;
    static int pulses_remained = 0;                 // ќсталось импульсов
    static bool in_pause = false;
    static uint period_heavy_impulses = 60 * 1000;  // ¬рем€ между "т€жЄлыми" импульсами - теми, которые выдел€ют много тепла (5а, 5б)
    static uint time_last_heavy_impulse = 0;        // ¬рем€ запуска последнего "т€жЄлого" импульса

    static TypeSignal::E current = TypeSignal::Count;

    int RemainedPulses()
    {
        return pulses_remained;
    }

    void SendSequence(int num_step, int volts, int ms);
}


void IT6523::Update()
{
    // \todo Ёти пл€ски нужны потому, что сразу после подключени€ к серверу он не получает команду
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


bool IT6523::Start(TypeSignal::E type, int num_pulses)
{
    current = type;

    pulses_remained = num_pulses;

    uint period = 5000;

    char buffer[32];

    if (current == TypeSignal::_2b_SAEJ1113)
    {
        IT6523::SendCommandF("RES 0.01");
        IT6523::SendCommandF("carwave:sae:2b:volt %dV", gset.voltage_mode.CurrentVolts());
//        IT6523::SendCommandF("carwave:sae:2b:TD %s", SU::MilliUnitsToUnits(PageSignal2b::param_td.GetValue().ToMU(), buffer));
        IT6523::SendCommand("carwave:sae:2b:state 1");
    }
    else if (current == TypeSignal::_4_DIN40839)
    {
        /*
        IT6523::SendCommandF("carwave:startup:din40839:volt %dV", (gset.voltage_mode == VoltageMode::_12) ? 12 : 24);
        IT6523::SendCommand("carwave:startup:din40839:state 1");
        */

        IT6523::SendCommandF("RES 0.01");
        IT6523::SendCommand("list:state 0");
        IT6523::SendCommand("sequence:edit");
        IT6523::SendCommand("sequence:recall 1");
        IT6523::SendCommand("sequence:step:count 3");

//        SU::MilliUnitsToUnits(gset.voltage_mode.CurrentVolts() * 1000 - PageSignal4::param_Us.GetValue().ToMU(), buffer);
//        int duration = PageSignal4::param_t7.GetValue().ToMU();
//        IT6523::SendCommandF("seq:volt 1,%s; curr 1,60; width 1,%d ms; slop 1,10 ms; load curr 1,3", buffer, duration);

//        SU::MilliUnitsToUnits(gset.voltage_mode.CurrentVolts() * 1000 - PageSignal4::param_Ua.GetValue().ToMU(), buffer);
//        duration = PageSignal4::param_t9.GetValue().ToMU();
//        IT6523::SendCommandF("seq:volt 2,%s; curr 2,60; width 2,%d ms; slop 2,10 ms; load curr 2,3", buffer, duration);

        SU::MilliUnitsToUnits(gset.voltage_mode.CurrentVolts() * 1000, buffer);
//        IT6523::SendCommandF("seq:volt 3,%s; curr 3,60; width 2,%d ms; slop 3,10 ms; load curr 3,3", buffer, duration);

        IT6523::SendCommand("sequence:save 1");

        IT6523::SendCommand("list:edit 1");
        IT6523::SendCommand("list:repeat 1");
        IT6523::SendCommand("list:sequence:count 1");
        IT6523::SendCommand("list:sequence:select 1");
        IT6523::SendCommand("list save 1");

        IT6523::SendCommand("list:recall 1");
        IT6523::SendCommand("list:state 1");

        period = 10000;
    }
    else if (current == TypeSignal::_5a_16750_1)
    {
        if (time_last_heavy_impulse != 0 && TIME_MS < time_last_heavy_impulse + period_heavy_impulses)
        {
            return false;
        }

        period = period_heavy_impulses;
//        IT6523::SendCommandF("RES %d.0", PageSignal5a::param_Ri.GetValue().ToMU());
        IT6523::SendCommand("carwave:iso16750:load:dump:test amode");
        IT6523::SendCommandF("carwave:iso16750:load:dump:volt %dV", gset.voltage_mode.CurrentVolts());
//        IT6523::SendCommandF("carwave:iso16750:load:dump:UN %d", PageSignal5a::param_Us.GetValue().ToMU());
//        IT6523::SendCommandF("carwave:iso16750:load:dump:TD %s", SU::MilliUnitsToUnits(PageSignal5a::param_td.GetValue().ToMU(), buffer));
        IT6523::SendCommandF("carwave:iso16750:load:dump:state 1");
        time_last_heavy_impulse = TIME_MS;
    }
    else if (current == TypeSignal::_5b_16750_2)
    {
        if (time_last_heavy_impulse != 0 && TIME_MS < time_last_heavy_impulse + period_heavy_impulses)
        {
            return false;
        }

        period = period_heavy_impulses;
//        IT6523::SendCommandF("RES %d.0", PageSignal5b::param_Ri.GetValue().ToMU());
        IT6523::SendCommand("carwave:iso16750:load:dump:test bmode");
        IT6523::SendCommandF("carwave:iso16750:load:dump:volt %dV", gset.voltage_mode.CurrentVolts());
//        IT6523::SendCommandF("carwave:iso16750:load:dump:UN %d", PageSignal5b::param_Us.GetValue().ToMU());
        IT6523::SendCommandF("carwave:iso16750:load:dump:US %d", gset.voltage_mode.Is12() ? 35 : 59);
//        IT6523::SendCommandF("carwave:iso16750:load:dump:TD %s", SU::MilliUnitsToUnits(PageSignal5b::param_td.GetValue().ToMU(), buffer));
        IT6523::SendCommandF("carwave:iso16750:load:dump:state 1");
        time_last_heavy_impulse = TIME_MS;
    }

    SendCommand("SOURCE:OUTPut:STATE 1");

    Timer::SetPeriodicTask(TimerTask::IT6523, period, IT6523::CallbackOnTimerImpulse);

    return true;
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

    Keyboard::AddKey(Key::Stop);
}


uint IT6523::TimeLeftToHeavyImpulse()
{
    if (TIME_MS >= time_last_heavy_impulse + period_heavy_impulses)
    {
        return 0;
    }

    return (period_heavy_impulses - (TIME_MS - time_last_heavy_impulse)) / 1000;
}
