// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"
#include "Utils/String.h"

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
        Back,       // 16
        OK,         // 17
        GovButton,  // 18
        GovLeft,    // 19
        GovRight,   // 20
        Count
    };

    static String Name(E);
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
};

namespace Keyboard
{
    bool Init();
    // ���������� true, ���� ���� ��������
    bool Empty();
    // ���������� ��������� �������
    Control NextControl();

    void AppendControl(const Control &control);

    void Lock();

    void Unlock();
};