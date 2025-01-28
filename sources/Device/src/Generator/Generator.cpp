// 2024/02/01 23:53:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Generator.h"
#include "Generator/FPGA.h"
#include "Generator/SwitchingBoard.h"
#include "Generator/MCP4811.h"
#include "Generator/MAX532.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Connector/Device/Value_.h"


void Generator::Stop()
{
    FPGA::Stop();

    MCP4811::Disable();

    FPGA::SetTypeSignal(TypeSignal::Off);
}


void Generator::Pause()
{
    FPGA::Pause();
}


void Generator::Resume()
{
    FPGA::Resume();
}


void Generator::Start1_12V(const Value &Us, const Value &t1)
{
    Stop();

    FPGA::SetTypeSignal(TypeSignal::_1_12V); 

    FPGA::WritePeriod(TypeSignal::_1_12V, t1);
    
    MCP4811::SetVoltage(Us);

    FPGA::Start();
}


void Generator::Start1_24V(const Value &Us, const Value &t1)
{
    Stop();

    FPGA::SetTypeSignal(TypeSignal::_1_24V);

    FPGA::WritePeriod(TypeSignal::_1_24V, t1);
    
    MCP4811::SetVoltage(Us);

    FPGA::Start();
}


void Generator::Start2A(const Value &Us, const Value &t1)
{
    Stop();

    FPGA::SetTypeSignal(TypeSignal::_2a);

    FPGA::WritePeriod(TypeSignal::_2a, t1);

    MCP4811::SetVoltage(Us);

    FPGA::Start();
}


void Generator::Start3A(const Value &Us)
{
    Stop();

    FPGA::SetTypeSignal(TypeSignal::_3a);

    MCP4811::SetVoltage(Us);

    FPGA::Start();
}


void Generator::Start3B(const Value &Us)
{
    Stop();

    FPGA::SetTypeSignal(TypeSignal::_3b);

    MCP4811::SetVoltage(Us);

    FPGA::Start();
}
