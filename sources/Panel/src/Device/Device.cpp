// 2024/02/01 10:16:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Device.h"
#include "Connector/Messages.h"
#include "Menu/MenuItems.h"


namespace Device
{
    static bool is_running = false;
}


void Device::Run()
{
    if (!is_running)
    {
        Page::ForCurrentSignal()->StartTest();
    }

    is_running = true;
}


void Device::Stop()
{
    if (is_running)
    {
        Message::Stop().Send();
    }

    is_running = false;
}


bool Device::IsRunning()
{
    return is_running;
}
