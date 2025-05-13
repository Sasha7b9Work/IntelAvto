// 2024/01/04 10:55:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


namespace PageMain
{
    extern Page *self;
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
    extern Page *self;
}


namespace PageSignal2a
{
    extern Page *self;
}


namespace PageSignal2b
{
    extern Page *self;

    extern TParameter param_td;
}


namespace PageSignal3a
{
    extern Page *self;
}


namespace PageSignal3b
{
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
