// 2024/02/02 00:03:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA.h"
#include "Hardware/HAL/HAL_PINS.h"
//#include "Utils/Log.h"


TypeSignal::E TypeSignal::current = TypeSignal::Count;


namespace FPGA
{
    struct Reg
    {
        enum E
        {
            Duration1   = 0x01,
            Delaty1     = 0x02,
            AccumOff    = 0x03,
            Period1     = 0x04,

            Diration2a  = 0x05,
            Period2a    = 0x06,

            Duration3ab = 0x07,
            Period3ab   = 0x08,

            Fail = 0xFF
        };

        Reg(E a) : address(a) { }

        static Reg ForPeriod();

        void Write(const Value &);

    private:

        E address;
    };
}


void FPGA::SetTypeSignal()
{
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
    else
    {
        pin_NPULSE2.ToHi();
    }
}


void FPGA::WritePeriod(const Value &period)
{
    Reg::ForPeriod().Write(period);
}


void FPGA::Start()
{

}


FPGA::Reg FPGA::Reg::ForPeriod()
{
    if (TypeSignal::Is1())
    {
        return Reg(Reg::Period1);
    }
    else if (TypeSignal::Is2a())
    {
        return Reg(Reg::Period2a);
    }
    else if (TypeSignal::Is3a() || TypeSignal::Is3b())
    {
        return Reg(Reg::Period3ab);
    }

    return Reg(Reg::Fail);
}


void FPGA::Reg::Write(const Value &value)
{
    if (address == Reg::Fail)
    {
//        LOG_ERROR("Address register fail");
    }
}
