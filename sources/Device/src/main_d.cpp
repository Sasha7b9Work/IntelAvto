// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"
#include "Generator/Generator.h"
#include "Generator/MAX532.h"
#include "Connector/Device/Interface_d.h"
#include "Connector/Device/Value_.h"


int main()
{
    HAL::Init();
    HAL_TIM::DelayMS(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию

    Generator::Stop();

    while (1)
    {
        Generator::Start2A(Value(0, TypeValue::Voltage), Value(1, TypeValue::Time));

        HAL_TIM::DelayMS(5000);

        Generator::Stop();

        HAL_TIM::DelayMS(5000);

//        DInterface::Update();
    }
}
