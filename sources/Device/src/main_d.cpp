// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"
#include "Generator/Generator.h"
#include "Generator/MAX532.h"
#include "Connector/Device/Interface_d.h"
#include "Connector/Device/Value_.h"
#include "Generator/FPGA.h"


int main()
{
    HAL::Init();
    HAL_TIM::DelayMS(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию

    Generator::Stop();

    FPGA::Init();

    while (1)
    {
        DInterface::Update();

//        FPGA::WritePeriod(TypeSignal::_1_12V, Value(1000));
//        FPGA::WritePeriod(TypeSignal::_2a, Value(1000));
//
//        FPGA::Start();
//
//        TimeMeterMS().Delay(5000);
//
//        FPGA::Stop();
//
//        TimeMeterMS().Delay(5000);
    }
}
