// 2024/02/01 23:53:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Generator.h"
#include "Generator/FPGA.h"
#include "Generator/SwitchingBoard.h"
#include "Generator/MCP4811.h"
#include "Generator/MAX532.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL_PINS.h"


void Generator::Stop()
{
    FPGA::SetTypeSignal(TypeSignal::Off);

    FPGA::Stop();
}


void Generator::Start1(const Value &Us, const Value &t1)
{
    if (TypeSignal::IsOff())
    {
        FPGA::SetTypeSignal(TypeSignal::_1);

        FPGA::WritePeriod(t1);

        MCP4811::SetVoltage(Us);

        FPGA::Start();
    }
}


void Generator::Start2A(const Value &Us, const Value &t1)
{
    if (TypeSignal::IsOff())
    {
        FPGA::SetTypeSignal(TypeSignal::_2a);

        FPGA::WritePeriod(t1);

        MCP4811::SetVoltage(Us);

        FPGA::Start();
    }
}


void Generator::Start3A(const Value &Us)
{
    if (TypeSignal::IsOff())
    {
        FPGA::SetTypeSignal(TypeSignal::_3a);

        MCP4811::SetVoltage(Us);

        FPGA::Start();
    }
}


void Generator::Start3B(const Value &Us)
{
    if (TypeSignal::IsOff())
    {
        FPGA::SetTypeSignal(TypeSignal::_3b);

        MCP4811::SetVoltage(Us);

        FPGA::Start();
    }
}
