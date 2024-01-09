// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Unit
{
    enum E
    {
        Seconds,
        Volts,
        Null,
        Count
    };
};


struct Value
{
    Value(int _munits = 0, Unit::E u = Unit::Null) : munits(_munits), unit(u)  { }

    void Draw(int x, int y);

private:

    int munits;     // Значение в миллиюнитах (например, 1 = 1 мс)

    Unit::E unit;
};


struct Voltage : public Value
{
    Voltage(int _munits) : Value(_munits, Unit::Volts) {}
};


struct Time : public Value
{
    Time(int _munits) : Value(_munits, Unit::Seconds) {}
};
