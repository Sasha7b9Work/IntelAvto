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
    HAL_TIM::DelayMS(500);             // �������� ����� ��� ����, ����� AD9952 ����� ������ ���������� �������������

    Generator::Stop();

    while (1)
    {
        DInterface::Update();
    }
}
