// 2024/02/02 00:02:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Value_.h"


struct TypeSignal
{
    enum E
    {
        _1,     // Int
        _2a,    // Int
        _3a,    // Int
        _3b,    // Int
        Off,
        Count
    };

    static void Set(E v) { current = v; }

    static bool Is1()  { return current == _1; }
    static bool Is2a() { return current == _2a; }
    static bool Is3a() { return current == _3a; }
    static bool Is3b() { return current == _3b; }
    static bool IsOff() { return current == Off; }

private:

    static E current;
};



namespace FPGA
{
    void Start();

    void Stop();

    void SetTypeSignal(TypeSignal::E);

    void WritePeriod(const Value &);

    void WriteDuration(const Value &);
}
