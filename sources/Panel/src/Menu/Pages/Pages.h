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

    extern VParameter param_Us;

    extern TParameter param_t1;
}


namespace PageSignal2a
{
    extern Page *self;

    extern VParameter param_Us;

    extern TParameter param_t1;
}


namespace PageSignal2b
{
    extern Page *self;
}


namespace PageSignal3a
{
    extern Page *self;

    extern VParameter param_Us;
}


namespace PageSignal3b
{
    extern Page *self;

    extern VParameter param_Us;
}


namespace PageSignal4
{
    extern VParameter param_Us;

    extern VParameter param_Ua;

    extern TParameter param_t7;

    extern TParameter param_t9;

    extern Page *self;
}


namespace PageSignal5a
{
    extern VParameter param_Us;

    extern TParameter param_td;

    extern Page *self;
}


namespace PageSignal5b
{
    extern VParameter param_Us;

    extern VParameter param_Uss;

    extern TParameter param_td;

    extern Page *self;
}
