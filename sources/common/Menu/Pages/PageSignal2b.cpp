// 2024/01/04 11:55:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Connector/Messages.h"


namespace PageSignal2b
{
    Parameter param_Us("Us", "Us",
        &gset.signals[TypeSignal::_2b].values12[0], Value(0), Value(10000),
        &gset.signals[TypeSignal::_2b].values24[0], Value(0), Value(10000),
        430, 170);

    Parameter param_td("td", "td",
        &gset.signals[TypeSignal::_2b].values12[1], Value(0), Value(10000),
        &gset.signals[TypeSignal::_2b].values24[2], Value(0), Value(10000),
        290, 60);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal3a::self);
        TypeSignal::Set(TypeSignal::_3a);
    }

    DEF_BUTTON(bSignal2b,
        "Сигнал", "Signal",
        FuncPress_Signal
    );

    static void FuncStartTest()
    {
        BaseMessage(Command::START_2B).Transmit();
    }

    static Item *items[] =
    {
        &bSignal2b,
        &param_Us,
        &param_td,
        nullptr
    };

    static Page page(items, nullptr, nullptr, FuncStartTest);
}


Page *PageSignal2b::self = &PageSignal2b::page;
