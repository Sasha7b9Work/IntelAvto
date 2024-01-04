// 2024/01/04 14:05:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"


namespace PageSignal3b
{
    static Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_3b].values[0]);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal4::self);
        TypeSignal::Set(TypeSignal::_4);
    }

    DEF_BUTTON(bSignal3b,
        "3b", "3b",
        FuncPress_Signal
    );

    static Item *items[] =
    {
        &bSignal3b,
        &param_Us,
        nullptr
    };

    static Page page(items, nullptr, nullptr);
}


Page *PageSignal3b::self = &PageSignal3b::page;
