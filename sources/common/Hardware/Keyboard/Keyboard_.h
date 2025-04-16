// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


struct Key
{
    enum E
    {
        None,       //  0
        _0,         //  1
        _1,         //  2
        _2,         //  3
        _3,         //  4
        _4,         //  5
        _5,         //  6
        _6,         //  7
        _7,         //  8
        _8,         //  9
        _9,         // 10
        Minus,      // 11
        Dot,        // 12
        Start,      // 13
        Left,       // 14
        Right,      // 15
        Esc,        // 16
        OK,         // 17
        Stop,       // 21
        GovButton,  // 18
        GovLeft,    // 19
        GovRight,   // 20
        Count
    };

    static pchar Name(E);
};


namespace Keyboard
{
    bool Init();

    // Возвращает true, если есть событыия
    bool Empty();

    // Возвращает следующее событие
    Key::E NextKey();

    void Lock();

    void Unlock();

    // Добавить действие в буфер
    void AddKey(Key::E);
};
