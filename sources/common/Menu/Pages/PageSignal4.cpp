// 2024/01/04 14:11:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"


namespace PageSignal4
{
    Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_4].values[0], 230, 110);

    Parameter param_Ua("Ua", "Ua", gset.signals[TypeSignal::_4].values[1], 350, 100);

    Parameter param_t7("t7", "t7", gset.signals[TypeSignal::_4].values[2], 220, 160);

    Parameter param_t9("t9", "t9", gset.signals[TypeSignal::_4].values[3], 330, 160);

    Parameter param_t11("t11", "t11", gset.signals[TypeSignal::_4].values[4], 410, 160);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal5a::self);
        TypeSignal::Set(TypeSignal::_5a);
    }

    DEF_BUTTON(bSignal5a,
        "4", "4",
        FuncPress_Signal
    );

    static Item *items[] =
    {
        &bSignal5a,
        &param_Us,
        &param_Ua,
        &param_t7,
        &param_t9,
        &param_t11,
        nullptr
    };

    static Page page(items, nullptr, nullptr);
}


Page *PageSignal4::self = &PageSignal4::page;
