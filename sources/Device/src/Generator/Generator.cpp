// 2024/02/01 23:53:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Generator.h"
#include "Generator/FPGA.h"
#include "Generator/SwitchingBoard.h"
#include "Generator/VoltageDAC.h"
#include "Hardware/Timer.h"


void Generator::Start2A(const Value &Us, const Value &t1)
{
    TypeSignal::Set(TypeSignal::_2a);

    FPGA::SetTypeSignal();

    FPGA::WritePeriod(t1);

    SwitchingBoard::SetTypeSignal();

    VoltageDAC::SetZero();

    VoltageDAC::SetOutputSource(Us);

    TimeMeterMS().Delay(1000);

    FPGA::Start();
}
