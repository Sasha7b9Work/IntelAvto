// 2024/02/02 00:21:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/MCP4801.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/HAL/HAL.h"


namespace MCP4801
{
    struct Converter
    {
        Converter(const Value &_value) : value(_value) { }
        uint16 Resolve() const;
    private:
        Value value;
    };

    namespace Gateway
    {
        static void Write(uint16);
        static void WriteBit(bool);
    }
}


void MCP4801::Test()
{
    uint16 value = 0;

    while (true)
    {
        Gateway::Write(value++);
        HAL_TIM::DelayUS(10);
    }
}


void MCP4801::Set(const Value &value)
{
    Gateway::Write(Converter(value).Resolve());
}


uint16 MCP4801::Converter::Resolve() const
{
    /*
    * 
    *   Vout = (2.048f * X) * G / (2 ** n) * 200
    * 
    *  X = (2 ** n) * Vout / 2.048f / G / 200
    *  
    */

    return (uint16)(1024 * value.ToFloat() / 2.048f / 2.0f / 200.0f);
}


void MCP4801::Gateway::Write(uint16 value)
{
    pin_DAC_CS2_R.ToLow();

    WriteBit(false);
    WriteBit(false);
    WriteBit(true);
    WriteBit(true);

    for (int i = 9; i >= 0; i--)
    {
        WriteBit((value & (1 << 9)) != 0);
    }

    WriteBit(false);
    WriteBit(false);

    pin_DAC_CS2_R.ToHi();

    WriteBit(false);
}


void MCP4801::Gateway::WriteBit(bool bit)
{
    bit ? pin_DAC_DAT_R.ToHi() : pin_DAC_DAT_R.ToLow();

    HAL_TIM::DelayUS(5);

    pin_DAC_SCLK_R.ToHi();

    HAL_TIM::DelayUS(10);

    pin_DAC_SCLK_R.ToLow();

    HAL_TIM::DelayUS(5);
}
