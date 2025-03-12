// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


struct Key
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


struct Action
{
    enum E
    {
        Press,
        Release,
        Long,
        Count
    };
};

struct Control
{
    Key::E key = Key::None;
    Action::E action = Action::Count;

    Control(Key::E v = Key::None, Action::E a = Action::Press) : key(v), action(a) {}

    bool IsRotateGovernor() const { return (key == Key::GovLeft) || (key == Key::GovRight); }

    bool IsRelease() const { return action == Action::Release; }
};

namespace Keyboard
{
    bool Init();

    // Возвращает true, если есть событыия
    bool Empty();

    // Возвращает следующее событие
    Control NextControl();

    void Lock();

    void Unlock();

    // Добавить действие в буфер
    void AddAction(Key::E, Action::E);
};
