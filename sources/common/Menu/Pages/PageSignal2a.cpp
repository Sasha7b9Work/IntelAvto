// 2024/01/04 11:26:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"


namespace PageSignal2a
{
    Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_2a].values[0], 320, 140);

    Parameter param_t1("t1", "t1", gset.signals[TypeSignal::_2a].values[1], 270, 30);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal2b::self);
        TypeSignal::Set(TypeSignal::_2b);
    }

    DEF_BUTTON(bSignal2a,
        "������", "Signal",
        FuncPress_Signal
    );

    static Item *items[] =
    {
        &bSignal2a,
        &param_Us,
        &param_t1,
        nullptr
    };

    static Page page(items, nullptr, nullptr);
}


Page *PageSignal2a::self = &PageSignal2a::page;
