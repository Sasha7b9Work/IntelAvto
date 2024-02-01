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
};



namespace FPGA
{
    void SetTypeSignal(TypeSignal::E);

    void WritePeriod(const Value &);
}
