// 2024/02/02 00:03:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Generator/SwitchingBoard.h"
#include "Connector/Device/Value_.h"


TypeSignal::E TypeSignal::current = TypeSignal::Off;


namespace FPGA
{
    struct Reg
    {
        enum E
        {
            None      = 0x00,
            Period1   = 0x01,       // Период импульса 1 - должен быть больше 500 мс
            Period2   = 0x03,       // Период импульса 2 - должен быть больше 500 мс
            Fail,
            Count,
        };

        Reg(E a) : address(a) { }

        static Reg ForPeriod(TypeSignal::E type)
        {
            if (type == TypeSignal::_1_12V || type == TypeSignal::_1_24V)
            {
                return Reg(Reg::Period1);
            }
            else if (type == TypeSignal::_2a)
            {
                return Reg(Reg::Period2);
            }

            return Reg(Reg::Fail);
        }

        void Write(const Value &);

    private:

        const E address;

        void SetAddress(uint8);

        void WriteRawValue(uint);
    };
}


void FPGA::SetTypeSignal(TypeSignal::E type)
{
    TypeSignal::Set(type);

    static const bool states[TypeSignal::Count][3] =
    {
        { true,  false, false },
        { false, true,  false },
        { true,  true,  false },
        { false, false, true },
        { true,  false, true },
        { false, false, false }
    };

    static PinOut *pins[3] = { &pin_NPULES0, &pin_NPULSE1, &pin_NPULSE2 };

    for (int i = 0; i < 3; i++)
    {
        pins[i]->ToState(states[type][i]);
    }

    SwitchingBoard::SetTypeSignal();
}


void FPGA::WritePeriod(TypeSignal::E type, const Value &period)
{
    Reg::ForPeriod(type).Write(period);
}


void FPGA::Start()
{
    pin_START.ToHi();

    HAL_TIM::DelayUS(20);

    pin_START.ToLow();

    Resume();

    TimeMeterMS().Delay(2);
}


void FPGA::Stop()
{
    pin_STOP.ToHi();

    HAL_TIM::DelayUS(20);

    pin_STOP.ToLow();

    Pause();
}


void FPGA::Init()
{
    WritePeriod(TypeSignal::_1_12V, Value(1000));
    WritePeriod(TypeSignal::_2a, Value(1000));
}


void FPGA::Pause()
{
    pin_ON_OFF.ToLow();
}


void FPGA::Resume()
{
    pin_ON_OFF.ToHi();
}


void FPGA::Reg::Write(const Value &value)
{
    if (address == Reg::Fail)
    {
//        LOG_ERROR("Address register fail");
        return;
    }

    SetAddress((uint8)address);

    uint duration = (uint)value.ToInt() * 1000;         // Число приходит в миллисекундах, переводим его в микросекунды перед записью

    WriteRawValue(duration);

    pin_WR_RG.ToHi(5);

    SetAddress(0);
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
    for (int i = 23; i >= 0; i--)
    {
        pin_DAT_RG.ToState((value & (1 << i)) != 0);

        HAL_TIM::DelayUS(5);

        pin_CLK_RG.ToHi();

        HAL_TIM::DelayUS(10);

        pin_CLK_RG.ToLow();

        HAL_TIM::DelayUS(5);
    }
}
