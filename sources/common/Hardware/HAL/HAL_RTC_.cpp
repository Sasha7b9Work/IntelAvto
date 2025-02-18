// 2025/02/04 09:00:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


PackedTime HAL_RTC::GetPackedTime()
{
    PackedTime time;

    std::memset(&time, 0, sizeof(time));

    return time;
}
