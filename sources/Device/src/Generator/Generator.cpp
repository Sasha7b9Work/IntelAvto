// 2024/02/01 23:53:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Generator.h"
#include "Generator/FPGA.h"
#include "Generator/SwitchingBoard.h"
#include "Generator/VoltageOut.h"
#include "Generator/SourceFollower.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL_PINS.h"


void Generator::Stop()
{
    VoltageOut::Set(Value(0));

    pin_K1_FOR.ToLow();
    pin_K2_FOR.ToLow();

    FPGA::Stop();
}


void Generator::Start2A(const Value &Us, const Value &t1)
{
    TypeSignal::Set(TypeSignal::_2a);

    FPGA::SetTypeSignal();

    FPGA::WritePeriod(t1);

    SwitchingBoard::SetTypeSignal();

    VoltageOut::Set(Value(0));

    pin_K1_FOR.ToHi();
    pin_K2_FOR.ToHi();

    TimeMeterMS().Delay(1000);

    SourceFollower::Set(Us);
    VoltageOut::Set(Us);

    TimeMeterMS().Delay(1000);

    FPGA::Start();
}
