// 2024/02/02 00:21:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/MCP4811.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/HAL/HAL.h"


namespace MCP4811
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


void MCP4811::Test()
{
    uint16 value = 0;

    while (true)
    {
        Gateway::Write(value++);
        HAL_TIM::DelayUS(10);
    }
}


void MCP4811::SetVoltage(const Value &value)
{
    Gateway::Write(Converter(value).Resolve());

    pin_DAC_ENB_HV.ToHi();
}


void MCP4811::Disable()
{
    Gateway::Write(Converter(Value(0, TypeValue::Voltage)).Resolve());

    pin_DAC_ENB_HV.ToLow();
}


uint16 MCP4811::Converter::Resolve() const
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


void MCP4811::Gateway::Write(uint16 value)
{
    HAL_TIM::DelayUS(100);    // \todo    на макете, кажется, без этого не работало

    // page 22 datasheet от 2010-го года

    WriteBit(false);
    WriteBit(false);
    WriteBit(false);    // GA
    WriteBit(true);     // SHDN

    for (int i = 9; i >= 0; i--)
    {
        WriteBit((value & (1 << i)) != 0);
    }

    WriteBit(false);
    WriteBit(false);

    WriteBit(false);    // Не помню, зачем
}


void MCP4811::Gateway::WriteBit(bool bit)
{
    bit ? pin_DAC_DAT_R.ToHi() : pin_DAC_DAT_R.ToLow();

    HAL_TIM::DelayUS(5);

    pin_DAC_SCLK_R.ToHi();

    HAL_TIM::DelayUS(10);

    pin_DAC_SCLK_R.ToLow();

    HAL_TIM::DelayUS(5);
}
