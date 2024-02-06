// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"


int main()
{
    CPU::Init();
    HAL_TIM::DelayMS(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию
  
    while (1)
    {
    }
}
