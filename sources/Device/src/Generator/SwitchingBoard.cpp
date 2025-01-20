// 2024/02/02 00:21:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/SwitchingBoard.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Generator/FPGA.h"


void SwitchingBoard::SetOff()
{
    static const int NUM_PINS = 7;

    static PinOut *pins[NUM_PINS] =
    {
        &pin_A12, &pin_2A, &pin_3AB, &pin_K3_COM, &pin_K5_COM, &pin_K6_COM, &pin_K7_COM
    };

    for (int i = 0; i < NUM_PINS; i++)
    {
        pins[i]->ToLow();
    }
}


void SwitchingBoard::SetTypeSignal()
{
    static const uint8 states[TypeSignal::Count][8] =
    {
        { 1, 0, 0, 0, 0, 1, 1, 1 },
        { 1, 0, 0, 0, 0, 1, 1, 1 },
        { 0, 1, 0, 1, 0, 0, 1, 1 },
        { 0, 0, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 1, 1, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    static PinOut *pins[8] = { &pin_A12, &pin_2A, &pin_3AB, &pin_K3_COM, &pin_K4_COM, &pin_K5_COM, &pin_K6_COM, &pin_K7_COM };

    TypeSignal::E type = TypeSignal::Current();

    for (int i = 0; i < 8; i++)
    {
        pins[i]->ToState(states[type][i] == 0 ? false : true);
    }
}
