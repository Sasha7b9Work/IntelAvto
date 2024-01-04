// 2023/12/25 17:20:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"


namespace PageSignal1
{
    Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_1].values[0], 335, 155);

    Parameter param_t1("t1", "t1", gset.signals[TypeSignal::_1].values[1], 310, 33);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal2a::self);

        TypeSignal::Set(TypeSignal::_2a);
    }


    DEF_BUTTON(bSignal1,
        "1", "1",
        FuncPress_Signal
    );


    static Item *items[] =
    {
        &bSignal1,
        &param_Us,
        &param_t1,
        nullptr
    };

    static Page page(items, nullptr, nullptr);
}


Page *PageSignal1::self = &PageSignal1::page;
