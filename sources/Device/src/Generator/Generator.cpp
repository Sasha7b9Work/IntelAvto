// 2024/02/01 23:53:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Generator.h"
#include "Generator/FPGA.h"


void Generator::Start2A(const Value &Us, const Value &t1)
{
    FPGA::SetTypeSignal(TypeSignal::_2a);
}
