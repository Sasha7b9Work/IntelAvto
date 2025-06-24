// 2024/01/04 10:55:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


namespace PageMain
{
    extern Page *self;
}


namespace PageSettings
{
    extern Page *self;

    // Показывать засылаемое напряжение
    extern uint8 show_debug_voltage;
}


namespace PageCalibration
{
    extern Page *self;

    void UpdateInput();
}


namespace PageLAN
{
    extern Page *self;

    namespace PageIT6523
    {
        extern Page *self;
    }

    namespace PageSCPI
    {
        extern Page *self;
    }
}


namespace PageInfo
{
    extern Page *self;
}


namespace PageSignal1
{
    static const int us12_min = 75;
    static const int us12_max = 150;
    static const int us24_min = 300;
    static const int us24_max = 600;

    static const int current_min = 1;
    static const int current_max = 50;

    extern Page *self;
}


namespace PageSignal2a
{
    static const int us12_min = 37;
    static const int us12_max = 112;
    static const int us24_min = 37;
    static const int us24_max = 112;

    extern Page *self;
}


namespace PageSignal2b
{
    extern Page *self;

    extern TParameter param_td;
}


namespace PageSignal3a
{
    static const int us12_min = 112;
    static const int us12_max = 220;
    static const int us24_min = 150;
    static const int us24_max = 300;

    extern Page *self;
}


namespace PageSignal3b
{
    static const int us12_min = 75;
    static const int us12_max = 150;
    static const int us24_min = 150;
    static const int us24_max = 300;

    extern Page *self;
}


namespace PageSignal4
{
    extern Page *self;

    extern VParameter param_Us;

    extern VParameter param_Ua;

    extern TParameter param_t7;

    extern TParameter param_t9;
}


namespace PageSignal5a
{
    extern VParameter param_Us;

    extern TParameter param_td;

    extern CParameter param_Ri;

    extern Page *self;
}


namespace PageSignal5b
{
    extern Page *self;

    extern VParameter param_Us;

    extern TParameter param_td;

    extern CParameter param_Ri;
}
