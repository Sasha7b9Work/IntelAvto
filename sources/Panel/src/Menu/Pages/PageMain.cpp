// 2023/12/22 15:42:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Signals/PageSignals.h"
#include "Menu/MenuItemsDef.h"


TypeSignal PageSignals::typeSignal(TypeSignal::_1);


static void OnPress_TypeSignal()
{

}

// Выбор типа сигнала
DEF_SWITCH_8(sTypeSignal,
    "Сигнал", "Signal",
    "Время индикации", "Display time",
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
