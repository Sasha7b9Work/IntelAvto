// 2024/02/02 00:21:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/SwitchingBoard.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Generator/FPGA.h"


namespace SwitchingBoard
{
    /*
    +--------+---+---+---+---+---+---+---+---+
    | Сигнал |A12|2A |3AB|K1 |K3 |K5 |K6 |K7 |
    +--------+---+---+---+---+---+---+---+---+
    | OFF    |   |   |   |   |   |   |   |   |
    | 1      | 1 |   |   | 1 |   | 1 | 1 | 1 |
    | 2a     |   | 1 |   | 1 | 1 | 1 |   | 1 |
    | 3a,b   |   |   | 1 | 1 | 1 |   | 1 |   |
    +--------+---+---+---+---+---+---+---+---+
    */
}


void SwitchingBoard::SetOff()
{
    static const int NUM_PINS = 8;

    static PinOut *pins[NUM_PINS] =
    {
        &pin_A12, &pin_2A, &pin_3AB, &pin_K1_COM, &pin_K3_COM, &pin_K5_COM, &pin_K6_COM, &pin_K7_COM
    };

    for (int i = 0; i < NUM_PINS; i++)
    {
        pins[i]->ToLow();
    }
}


void SwitchingBoard::SetTypeSignal()
{
    SetOff();

    if (TypeSignal::Is1())
    {
        pin_A12.ToHi();
        pin_K1_COM.ToHi();
        pin_K5_COM.ToHi();
        pin_K6_COM.ToHi();
        pin_K7_COM.ToHi();
    }
    else if (TypeSignal::Is2a())
    {
        pin_2A.ToHi();
        pin_K1_COM.ToHi();
        pin_K3_COM.ToHi();
        pin_K5_COM.ToHi();
        pin_K7_COM.ToHi();
    }
    else if (TypeSignal::Is3a() || TypeSignal::Is3b())
    {
        pin_3AB.ToHi();
        pin_K1_COM.ToHi();
        pin_K3_COM.ToHi();
        pin_K6_COM.ToHi();
    }
}
