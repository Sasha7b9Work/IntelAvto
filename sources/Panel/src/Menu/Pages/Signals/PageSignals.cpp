// 2023/12/25 16:59:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Signals/PageSignals.h"
#include "Menu/MenuItemsDef.h"


TypeSignal PageSignals::typeSignal(TypeSignal::_1);

static void OnPress_TypeSignal();


// Выбор типа сигнала
DEF_SWITCH_8(sTypeSignal,
    "Сигнал", "Signal",
    "1", "2a", "2b", "3a", "3b", "4", "5a", "5b",
    "1", "2a", "2b", "3a", "3b", "4", "5a", "5b",
    PageSignals::typeSignal, OnPress_TypeSignal
);


Item *items[7] =
{
    &sTypeSignal,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};


static void OnPress_TypeSignal()
{
    if (PageSignals::typeSignal.Is(TypeSignal::_1))
    {
        PageSignals::_1::SetParameters(&items[1]);
    }
    else if (PageSignals::typeSignal.Is(TypeSignal::_2a))
    {
        PageSignals::_2a::SetParameters(&items[1]);
    }
    else if (PageSignals::typeSignal.Is(TypeSignal::_2b))
    {
        PageSignals::_2b::SetParameters(&items[1]);
    }
    else if (PageSignals::typeSignal.Is(TypeSignal::_3a))
    {
        PageSignals::_3a::SetParameters(&items[1]);
    }
    else if (PageSignals::typeSignal.Is(TypeSignal::_3b))
    {
        PageSignals::_3b::SetParameters(&items[1]);
    }
    else if (PageSignals::typeSignal.Is(TypeSignal::_4))
    {
        PageSignals::_4::SetParameters(&items[1]);
    }
    else if (PageSignals::typeSignal.Is(TypeSignal::_5a))
    {
        PageSignals::_5a::SetParameters(&items[1]);
    }
    else if (PageSignals::typeSignal.Is(TypeSignal::_5b))
    {
        PageSignals::_5b::SetParameters(&items[1]);
    }
}


void PageSignals::Init()
{
    OnPress_TypeSignal();
}

static Page pageIndication(items, nullptr, nullptr);

Page *PageSignals::self = &pageIndication;


pchar TypeSignal::Name(E v)
{
    static const pchar names[Count] =
    {
        "1",
        "2a",
        "SAEJ1113-11",
        "3a",
        "3b",
        "DIN40839",
        "16750-1",
        "16750-1"
    };

    return names[v];
}
