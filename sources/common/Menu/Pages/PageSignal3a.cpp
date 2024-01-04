// 2024/01/04 13:56:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Menu/MenuItemsDef.h"


namespace PageSignal3a
{
    Parameter param_Us("Us", "Us",
        gset.signals[TypeSignal::_3a].values12[0], Value(0), Value(10000),
        gset.signals[TypeSignal::_3b].values24[0], Value(0), Value(10000),
        275, 95);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal3b::self);
        TypeSignal::Set(TypeSignal::_3b);
    }

    DEF_BUTTON(bSignal3a,
        "Сигнал", "Signal",
        FuncPress_Signal
    );

    static Item *items[] =
    {
        &bSignal3a,
        &param_Us,
        nullptr
    };

    static Page page(items, nullptr, nullptr);
}


Page *PageSignal3a::self = &PageSignal3a::page;
