// 2023/12/25 16:59:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"



struct TypeSignal : public Enumeration
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
        Count
    };

    explicit TypeSignal(E v) : Enumeration((uint8)v) {}

    static pchar Name(E);
};


class PageSignals
{
public:
    static Page *self;

    static TypeSignal typeSignal;
};
