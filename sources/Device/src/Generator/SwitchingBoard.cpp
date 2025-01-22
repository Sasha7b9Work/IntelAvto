// 2024/02/02 00:21:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/SwitchingBoard.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Generator/FPGA.h"


namespace SwitchingBoard
{
    static const int NUM_PINS = 8;

    static PinOut *pins[NUM_PINS] =
    {
        &pin_A12, &pin_2A, &pin_3AB, &pin_K3_COM, &pin_K4_COM, &pin_K5_COM, &pin_K6_COM, &pin_K7_COM
    };
}


void SwitchingBoard::SetOff()
{
    for (int i = 0; i < NUM_PINS; i++)
    {
        pins[i]->ToLow();
    }
}


void SwitchingBoard::SetTypeSignal()
{
    static const uint8 states[TypeSignal::Count][NUM_PINS] =
    {
        { 1, 0, 0, 0, 0, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 1, 1, 1 },
        { 0, 1, 0, 1, 0, 0, 1, 1 },
        { 0, 0, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    TypeSignal::E type = TypeSignal::Current();

    for (int i = 0; i < NUM_PINS; i++)
    {
        pins[i]->ToState(states[type][i] != 0);
    }
}
