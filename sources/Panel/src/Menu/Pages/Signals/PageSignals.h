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


namespace PageSignals
{
    extern Page *self;

    extern TypeSignal typeSignal;

    void Init();

    namespace _1
    {
        void SetParameters(Item **first);
    }

    namespace _2a
    {
        void SetParameters(Item **first);
    }

    namespace _2b
    {
        void SetParameters(Item **first);
    }

    namespace _3a
    {
        void SetParameters(Item **first);
    }

    namespace _3b
    {
        void SetParameters(Item **first);
    }

    namespace _4
    {
        void SetParameters(Item **first);
    }

    namespace _5a
    {
        void SetParameters(Item **first);
    }

    namespace _5b
    {
        void SetParameters(Item **first);
    }
};
