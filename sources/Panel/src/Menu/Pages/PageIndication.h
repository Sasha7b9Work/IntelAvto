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
        Internal,           // Внутренний
        External            // Внешний
    };

    explicit RefGenerator(E v) : Enumeration((uint8)v) {};
    bool IsExternal() const { return (value == External); }
};


struct LaunchSource : public Enumeration
{
    enum E
    {
        Internal,           // Внутренний
        External,           // Внешний
        OneTime             // Однократный
    };

    explicit LaunchSource(E v) : Enumeration((uint8)v) {};
    bool IsExternal() const { return (value == External); }
    bool IsOneTime() const  { return (value == OneTime); }
};


struct MemoryMode : public Enumeration
{
    enum E
    { 
        Off,                // Выкл
        On                  // Вкл
    };

    explicit MemoryMode(E v) : Enumeration((uint8)v) {};
    bool IsOn() const { return (value == On); }
};


struct CalibrationMode : public Enumeration
{
    enum E
    {
        Disabled,       // Находимся не в режиме калибровки - ручка управляет уровнем синхронизации
        Enabled         // Находимся в режиме калибровки - ручка управляет калибровкой
    };

    explicit CalibrationMode(E v) : Enumeration((uint8)v) {};
    bool IsEnabled() const { return (value == Enabled); }
};


class PageIndication
{
public:
    static Page *self;

    static TypeSignal typeSignal;

    // Выбор опорного генератора
    static RefGenerator refGenerator;

    // Выбор источника запуска
    static LaunchSource launchSource;

    // Выбор 
    static CalibrationMode calibrationMode;

    // Включение(отключение) режима памяти
    static MemoryMode memoryMode;
    static bool OnceLaunch();
    static void OnceLaunchSwitchTrue();
    static void OnceLaunchSwitchFalse();
};
