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


bool Device::IsPause()
{
    return state == State::Paused;
}


void Device::_Start()
{
    Page::ForCurrentSignal()->StartTest();

    if (state == State::Stopped)
    {
        state = State::Running;
    }
    else if (state == State::Running)
    {
        state = State::Paused;
    }
    else if (state == State::Paused)
    {
        state = State::Running;
    }
}


void Device::Stop()
{
    Message::Stop().Transmit();

    state = State::Stopped;
}
