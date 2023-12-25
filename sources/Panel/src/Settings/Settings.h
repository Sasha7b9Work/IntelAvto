// 2023/12/25 17:58:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Value.h"
#include "Menu/Pages/Signals/PageSignals.h"


struct SettingsSignal
{
    static const int MAX_PARAMS = 5;

    Value values[MAX_PARAMS];
};


struct Settings
{
    SettingsSignal signals[TypeSignal::Count];
};
