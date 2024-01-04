// 2024/01/04 11:55:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"


namespace PageSignal2b
{
    Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_2b].values[0]);

    Parameter param_td("td", "td", gset.signals[TypeSignal::_2b].values[1]);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal3a::self);
        TypeSignal::Set(TypeSignal::_3a);
    }

    DEF_BUTTON(bSignal2b,
        "2b", "2b",
        FuncPress_Signal
    );

    static Item *items[] =
    {
        &bSignal2b,
        &param_Us,
        &param_td,
        nullptr
    };

    static Page page(items, nullptr, nullptr);
}


Page *PageSignal2b::self = &PageSignal2b::page;
