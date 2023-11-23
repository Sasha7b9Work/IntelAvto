// 2023/09/08 20:54:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


struct TypeSignal : public Enumeration
{
    enum E
    {
        _1,
        _2a,
        _2b,
        _3a,
        _3b,
        _4,
        _5a,
        _5b,
        Count
    };

    explicit TypeSignal(E v) : Enumeration((uint8)v) {};

    static pchar Name(E);
};

struct RefGenerator : public Enumeration
{
    enum E
    {
        Internal,           // ����������
        External            // �������
    };

    explicit RefGenerator(E v) : Enumeration((uint8)v) {};
    bool IsExternal() const { return (value == External); }
};


struct LaunchSource : public Enumeration
{
    enum E
    {
        Internal,           // ����������
        External,           // �������
        OneTime             // �����������
    };

    explicit LaunchSource(E v) : Enumeration((uint8)v) {};
    bool IsExternal() const { return (value == External); }
    bool IsOneTime() const  { return (value == OneTime); }
};


struct MemoryMode : public Enumeration
{
    enum E
    { 
        Off,                // ����
        On                  // ���
    };

    explicit MemoryMode(E v) : Enumeration((uint8)v) {};
    bool IsOn() const { return (value == On); }
};


struct CalibrationMode : public Enumeration
{
    enum E
    {
        Disabled,       // ��������� �� � ������ ���������� - ����� ��������� ������� �������������
        Enabled         // ��������� � ������ ���������� - ����� ��������� �����������
    };

    explicit CalibrationMode(E v) : Enumeration((uint8)v) {};
    bool IsEnabled() const { return (value == Enabled); }
};


class PageIndication
{
public:
    static Page *self;

    static TypeSignal typeSignal;

    // ����� �������� ����������
    static RefGenerator refGenerator;

    // ����� ��������� �������
    static LaunchSource launchSource;

    // ����� 
    static CalibrationMode calibrationMode;

    // ���������(����������) ������ ������
    static MemoryMode memoryMode;
    static bool OnceLaunch();
    static void OnceLaunchSwitchTrue();
    static void OnceLaunchSwitchFalse();
};
