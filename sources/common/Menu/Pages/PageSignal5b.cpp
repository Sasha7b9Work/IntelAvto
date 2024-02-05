// 2024/01/04 14:18:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Connector/Messages.h"


namespace PageSignal5b
{
    Parameter param_Us("Us", "Us",
        &gset.signals[TypeSignal::_5b].values12[0], Value(0), Value(10000),
        &gset.signals[TypeSignal::_5b].values24[0], Value(0), Value(10000),
        420, 130);

    Parameter param_Uss("Us*", "Us*",
        &gset.signals[TypeSignal::_5b].values12[1], Value(0), Value(10000),
        &gset.signals[TypeSignal::_5b].values24[1], Value(0), Value(10000),
        380, 160);

    Parameter param_td("td", "td",
        &gset.signals[TypeSignal::_5b].values12[2], Value(0), Value(10000),
        &gset.signals[TypeSignal::_5b].values24[2], Value(0), Value(10000),
        220, 160);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal1::self);
        TypeSignal::Set(TypeSignal::_1);
    }

    DEF_BUTTON(bSignal5b,
        "Сигнал", "Signal",
        FuncPress_Signal
    );

    static void FuncStartTest()
    {
        BaseMessage(Command::START_5B).Transmit();
    }

    static Item *items[] =
    {
        &bSignal5b,
        &param_Us,
        &param_Uss,
        &param_td,
        nullptr
    };

    static Page page(items, nullptr, nullptr, FuncStartTest);
}


Page *PageSignal5b::self = &PageSignal5b::page;
