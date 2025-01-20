// 2024/02/01 10:16:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Device.h"
#include "Connector/Messages_.h"
#include "Menu/MenuItems.h"


namespace Device
{
    struct State
    {
        enum E
        {
            Stopped,
            Running,
            Paused
        };
    };

    static State::E state;
}


bool Device::IsStopped()
{
    return state == State::Stopped;
}


bool Device::IsRunning()
{
    return state == State::Running;
}


bool Device::InPause()
{
    return state == State::Paused;
}


void Device::Start()
{
    if (IsStopped())
    {
        state = State::Running;

        Page::ForCurrentSignal()->StartTest();
    }
}


void Device::Pause()
{
    if (IsRunning())
    {
        state = State::Paused;

        Message::Stop().Transmit();
    }
}


void Device::Stop()
{
    if (Device::IsRunning() || Device::InPause())
    {
        state = State::Stopped;

        Message::Stop().Transmit();
    }
}
