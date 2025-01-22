// 2024/02/02 00:21:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/MCP4811.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Connector/Device/Value_.h"
#include <cmath>


namespace MCP4811
{
    static const uint DELAY_RELAYS = 10;        // Задержка на заряд/разряд

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


void MCP4811::SetVoltage(const Value &value)
{
    Gateway::Write(Converter(value).Resolve());

    pin_K1_FOR.ToHi();
    pin_DAC_ENB_HV.ToHi();

    TimeMeterMS().Delay(DELAY_RELAYS);
}


void MCP4811::Disable()
{
    pin_DAC_ENB_HV.ToLow();

    TimeMeterMS().Delay(DELAY_RELAYS);

    pin_K1_FOR.ToLow();

    Gateway::Write(Converter(Value(0, TypeValue::Voltage)).Resolve());
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

    return (uint16)(1024 * std::fabsf(value.ToFloat()) / 2.048f / 2.0f / 200.0f);
}


void MCP4811::Gateway::Write(uint16 value)
{
    pin_DAC_CS.ToLow();

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

    HAL_TIM::DelayUS(100);

    pin_DAC_CS.ToHi();

//    WriteBit(false);    // Не помню, зачем
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
