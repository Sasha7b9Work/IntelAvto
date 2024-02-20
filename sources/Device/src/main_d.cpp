// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"
#include "Connector/Interface_d.h"
#include "Generator/Generator.h"
#include "Generator/MAX532.h"


int main()
{
    HAL::Init();
    HAL_TIM::DelayMS(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию

    while (1)
    {
        MAX532::Test();

        DInterface::Update();

//        static uint time = 0;
//
//        if (TIME_MS - time > 1000)
//        {
//            time = TIME_MS;
//
//            Generator::Start2A(Value(10), Value(100), Value(10));
//        }
    }
}
