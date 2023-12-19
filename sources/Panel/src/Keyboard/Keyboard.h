// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Utils/String.h"


struct Control
{
    enum E
    {
        None,       //  0
        _1,         //  1
        _2,         //  2
        _3,         //  3
        _4,         //  4
        _5,         //  5
        _6,         //  6
        _7,         //  7
        _8,         //  8
        _9,         //  9
        _0,         // 10
        Minus,      // 11
        Dot,        // 12
        Start,      // 13
        Left,       // 14
        Right,      // 15
        Back,       // 16
        OK,         // 17
        GovButton,  // 18
        GovLeft,    // 19
        GovRight,   // 20
        Count
    } value;

    struct Action
    {
        enum E
        {
            Press,
            Release,
            Long
        } value;
        Action(E v) : value(v) {}
        bool IsPress() const { return value == Press; }
    } action;

    Control(E v = None, Action::E a = Action::Press) : value(v), action(a) {}

    String Name() const;

    bool IsRotateGovernor() const { return (value == GovLeft) || (value == GovRight); }
};

namespace Keyboard
{
    bool Init();
    // Возвращает true, если есть событыия
    bool Empty();
    // Возвращает следующее событие
    Control NextControl();

    void AppendControl(const Control &control);

    void Lock();

    void Unlock();
};
