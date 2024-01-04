// 2024/01/04 14:18:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"


namespace PageSignal5b
{
    Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_5b].values[0], 420, 130);

    Parameter param_Uss("Us*", "Us*", gset.signals[TypeSignal::_5b].values[1], 380, 160);

    Parameter param_td("td", "td", gset.signals[TypeSignal::_5b].values[2], 220, 160);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal1::self);
        TypeSignal::Set(TypeSignal::_1);
    }

    DEF_BUTTON(bSignal5b,
        "5b", "5b",
        FuncPress_Signal
    );

    static Item *items[] =
    {
        &bSignal5b,
        &param_Us,
        &param_Uss,
        &param_td,
        nullptr
    };

    static Page page(items, nullptr, nullptr);
}


Page *PageSignal5b::self = &PageSignal5b::page;
