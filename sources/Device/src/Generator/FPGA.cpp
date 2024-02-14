// 2024/02/02 00:03:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"


TypeSignal::E TypeSignal::current = TypeSignal::Count;


namespace FPGA
{
    struct Reg
    {
        enum E
        {
            Period1   = 0x01,       // ������ 500 ��
            Duration1 = 0x02,       // 1 - 20 ���

            Period2   = 0x03,       // ������ 500 ��
            Duration2 = 0x04,       // 1 - 20 ���

            Duration3 = 0x05,       // 200 - 5000 ��

            None = 0,

            Fail = 0xFF
        };

        Reg(E a) : address(a) { }

        static Reg ForPeriod()
        {
            if (TypeSignal::Is1())
            {
                return Reg(Reg::Period1);
            }
            else if (TypeSignal::Is2a() || TypeSignal::Is2b())
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
            else if (TypeSignal::Is2a() || TypeSignal::Is2b())
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

        // �� ����� ����� ����� �������� ������
        uint PeriodMul();

        void SetAddress();

        void WriteRawValue(uint);
    };
}


void FPGA::SetTypeSignal(TypeSignal::E type)
{
    TypeSignal::Set(type);

    /*
    +--------+------+------+------+
    | ������ |PULSE0|PULSE1|PULSE2|
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
    else if (TypeSignal::Is2a() || TypeSignal::Is2b())
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


void FPGA::Start()
{
    pin_START.ToHi();

    HAL_TIM::DelayUS(20);

    pin_START.ToLow();
}


void FPGA::Stop()
{
    pin_STOP.ToHi();

    HAL_TIM::DelayUS(20);

    pin_STOP.ToLow();
}


void FPGA::Reg::Write(const Value &value)
{
    if (address == Reg::Fail)
    {
//        LOG_ERROR("Address register fail");
        return;
    }

    pin_ON_OFF.ToLow();

    SetAddress();

    WriteRawValue((uint)value.GetRaw() * PeriodMul());

    pin_WR_RG.ToHi(5);

    pin_ON_OFF.ToHi();
}


uint FPGA::Reg::PeriodMul()
{
    if (TypeSignal::Is3a() || TypeSignal::Is3b())
    {
        return 10000U;
    }

    return 1000U;
}


void FPGA::Reg::SetAddress()
{
    static PinOut * const pins[4] =
    {
        &pin_A0_RG, &pin_A1_RG, &pin_A2_RG, &pin_A3_RG
    };

    for (int i = 0; i < 4; i++)
    {
        pins[i]->ToState((((uint8)address) & (1 << i)) != 0);
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
