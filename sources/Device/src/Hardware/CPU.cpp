// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>
#include <cstring>


namespace CPU
{
    // ¬рем€ последней установки состо€ни€ "зан€то"
    static uint timeBusy = 0;
}


void CPU::Init()
{
    HAL::Init();
}
