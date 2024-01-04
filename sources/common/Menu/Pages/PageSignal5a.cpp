// 2024/01/04 14:15:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"


namespace PageSignal5a
{
    Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_5a].values[0], 355, 125);

    Parameter param_td("td", "td", gset.signals[TypeSignal::_5a].values[1], 220, 155);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal5b::self);
        TypeSignal::Set(TypeSignal::_5b);
    }

    DEF_BUTTON(bSignal5a,
        "5a", "5a",
        FuncPress_Signal
    );

    static Item *items[] =
    {
        &bSignal5a,
        &param_Us,
        &param_td,
        nullptr
    };

    static Page page(items, nullptr, nullptr);
}


Page *PageSignal5a::self = &PageSignal5a::page;
