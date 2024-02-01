// 2024/01/04 14:15:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Connector/Messages.h"


namespace PageSignal5a
{
    Parameter param_Us("Us", "Us",
        &gset.signals[TypeSignal::_5a].values12[0], Value(0), Value(10000),
        &gset.signals[TypeSignal::_5a].values24[0], Value(0), Value(10000),
        355, 125);

    Parameter param_td("td", "td",
        &gset.signals[TypeSignal::_5a].values12[1], Value(0), Value(10000),
        &gset.signals[TypeSignal::_5a].values24[1], Value(0), Value(10000),
        220, 155);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal5b::self);
        TypeSignal::Set(TypeSignal::_5b);
    }

    DEF_BUTTON(bSignal5a,
        "������", "Signal",
        FuncPress_Signal
    );

    static void FuncStartTest()
    {
        Message(Command::START_5A).Send();
    }

    static Item *items[] =
    {
        &bSignal5a,
        &param_Us,
        &param_td,
        nullptr
    };

    static Page page(items, nullptr, nullptr, FuncStartTest);
}


Page *PageSignal5a::self = &PageSignal5a::page;
