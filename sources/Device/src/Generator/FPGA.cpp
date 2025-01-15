// 2024/02/02 00:03:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"


TypeSignal::E TypeSignal::current = TypeSignal::Off;


namespace FPGA
{
    struct Reg
    {
        enum E
        {
            None      = 0x00,
            Period1   = 0x01,       // Больше 500 мс
            Duration1 = 0x02,       // 1 - 20 мкс

            Period2   = 0x03,       // Больше 500 мс
            Duration2 = 0x04,       // 1 - 20 мкс

            Duration3 = 0x05,       // 200 - 5000 нс

            Fail,

            Count,
        };

        Reg(E a) : address(a) { }

        static Reg ForPeriod()
        {
            if (TypeSignal::Is1())
            {
                return Reg(Reg::Period1);
            }
            else if (TypeSignal::Is2a())
            {
                return Reg(Reg::Period2);
            }

            return Reg(Reg::Fail);
        }

        static Reg ForDiration()
        {
            if (TypeSignal::Is1())
            {
                return Reg(Reg::Duration1);
            }
            else if (TypeSignal::Is2a())
            {
                return Reg(Reg::Duration2);
            }
            else if (TypeSignal::Is3a() || TypeSignal::Is3b())
            {
                return Reg(Reg::Duration3);
            }

            return Reg(Reg::Fail);
        }

        void Write(const Value &);

    private:

        const E address;

        // На какое число нужно умножать период
        uint PeriodMul();

        void SetAddress(uint8);

        void WriteRawValue(uint);
    };
}


void FPGA::SetTypeSignal(TypeSignal::E type)
{
    TypeSignal::Set(type);

    /*
    +--------+------+------+------+
    | Сигнал |PULSE0|PULSE1|PULSE2|
    +--------+------+------+------+
    | OFF    |      |      |      |
    | 1      |  1   |      |      |
    | 2a     |      |  1   |      |
    | 3a,b   |  1   |  1   |      |
    | OFF    |      |      |  1   |
    +--------+------+------+------+
    */

    pin_NPULES0.ToLow();
    pin_NPULSE1.ToLow();
    pin_NPULSE2.ToLow();

    if (TypeSignal::Is1())
    {
        pin_NPULES0.ToHi();
    }
    else if (TypeSignal::Is2a())
    {
        pin_NPULSE1.ToHi();
    }
    else if (TypeSignal::Is3a() || TypeSignal::Is3b())
    {
        pin_NPULES0.ToHi();
        pin_NPULSE1.ToHi();
    }
}


void FPGA::WritePeriod(const Value &period)
{
    Reg::ForPeriod().Write(period);
}


void FPGA::WriteDuration(const Value &duration)
{
    Reg::ForDiration().Write(duration);
}


void FPGA::Start()
{
    pin_ON_OFF.ToHi();

    pin_START.ToHi();

    HAL_TIM::DelayUS(20);

    pin_START.ToLow();
}


void FPGA::Stop()
{
    pin_STOP.ToHi();

    HAL_TIM::DelayUS(20);

    pin_STOP.ToLow();

    pin_ON_OFF.ToLow();
}


void FPGA::Reg::Write(const Value &value)
{
    if (address == Reg::Fail)
    {
//        LOG_ERROR("Address register fail");
        return;
    }

    SetAddress((uint8)address);

    uint duration = (uint)value.ToInt() * PeriodMul();

    if (TypeSignal::Is3a() || TypeSignal::Is3b())
    {
        duration /= 100;
    }

    WriteRawValue(duration);

    pin_WR_RG.ToHi(5);

    SetAddress(0);
}


uint FPGA::Reg::PeriodMul()
{
    static const uint muls[] =
    {
        1,
        1000,       // Period1
        1,          // Duration1
        1000,       // Period2
        1,          // Duration2
        1           // Duration3
    };

    return address < 6 ? muls[address] : 0;
}


void FPGA::Reg::SetAddress(uint8 addr)
{
    static PinOut * const pins[4] =
    {
        &pin_A0_RG, &pin_A1_RG, &pin_A2_RG, &pin_A3_RG
    };

    for (int i = 0; i < 4; i++)
    {
        bool state = (((uint8)addr) & (1 << i)) != 0;

        pins[i]->ToState(state);
    }
}


void FPGA::Reg::WriteRawValue(uint value)
{
    for (int i = 31; i >= 0; i--)
    {
        pin_DAT_RG.ToState((value & (1 << i)) != 0);

        HAL_TIM::DelayUS(5);

        pin_CLK_RG.ToHi();

        HAL_TIM::DelayUS(10);

        pin_CLK_RG.ToLow();

        HAL_TIM::DelayUS(5);
    }
}
