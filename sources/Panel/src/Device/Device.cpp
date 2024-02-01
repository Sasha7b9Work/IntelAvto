// 2024/02/01 10:16:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/Device.h"


namespace Device
{
    static bool is_running = false;
}


void Device::Run()
{
    is_running = true;
}


void Device::Stop()
{
    is_running = false;
}


bool Device::IsRunning()
{
    return is_running;
}
