// 2024/02/02 00:03:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA.h"
#include "Hardware/HAL/HAL_PINS.h"


void FPGA::SetTypeSignal(TypeSignal::E type)
{
    pin_NPULES0.ToLow();
    pin_NPULSE1.ToLow();
    pin_NPULSE2.ToLow();

    if (type == TypeSignal::_1)
    {
        pin_NPULES0.ToHi();
    }
    else if (type == TypeSignal::_2a)
    {
        pin_NPULSE1.ToHi();
    }
    else if (type == TypeSignal::_3a || type == TypeSignal::_3b)
    {
        pin_NPULES0.ToHi();
        pin_NPULSE1.ToHi();
    }
    else
    {
        pin_NPULSE2.ToHi();
    }
}


void FPGA::WritePeriod(const Value &)
{

}


void FPGA::Start()
{

}
