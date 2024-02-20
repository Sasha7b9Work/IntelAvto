// 2024/02/04 18:34:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/MAX532.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/HAL/HAL.h"


namespace MAX532
{
    static void WriteChannels(uint);
    static void WriteBit(bool);
}


void MAX532::Set(const Value &)
{

}


void MAX532::Test()
{
    uint value = 0;

    while (true)
    {
        WriteChannels(value++);
    }
}


void MAX532::WriteChannels(uint value)
{
    pin_DAC_CS1_R.ToLow();

    for (int i = 23; i >= 0; i--)
    {
        WriteBit((value & (1 << i)) != 0);
    }

    pin_DAC_CS1_R.ToHi();
}


void MAX532::WriteBit(bool bit)
{
    bit ? pin_DAC_DAT_R.ToHi() : pin_DAC_DAT_R.ToLow();

    HAL_TIM::DelayUS(5);

    pin_DAC_SCLK_R.ToHi();

    HAL_TIM::DelayUS(10);

    pin_DAC_SCLK_R.ToLow();

    HAL_TIM::DelayUS(5);
}
