// 2024/02/02 00:02:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Value.h"


struct TypeSignal
{
    enum E
    {
        _1,     // Int
        _2a,    // Int
        _2b,    // Ext
        _3a,    // Int
        _3b,    // Int
        _4,     // Ext
        _5a,    // Ext
        _5b,    // Ext
        Off,
        Count
    };

    static void Set(E v) { current = v; }

    static bool Is1()  { return current == _1; }
    static bool Is2a() { return current == _2a; }
    static bool Is2b() { return current == _2b; }
    static bool Is3a() { return current == _3a; }
    static bool Is3b() { return current == _3b; }

private:

    static E current;
};



namespace FPGA
{
    void Start();

    void Stop();

    void SetTypeSignal();

    void WritePeriod(const Value &);
}
