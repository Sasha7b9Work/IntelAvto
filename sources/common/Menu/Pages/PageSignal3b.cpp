// 2024/01/04 14:05:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Connector/Messages.h"


namespace PageSignal3b
{
    Parameter param_Us("Us", "Us",
        &gset.signals[TypeSignal::_3b].values12[0], Value(0), Value(10000),
        &gset.signals[TypeSignal::_3b].values24[0], Value(0), Value(10000),
        350, 70);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal4::self);
        TypeSignal::Set(TypeSignal::_4);
    }

    DEF_BUTTON(bSignal3b,
        "Сигнал", "Signal",
        FuncPress_Signal
    );

    static void FuncStartTest()
    {
        Message(Command::START_3B).Send();
    }

    static Item *items[] =
    {
        &bSignal3b,
        &param_Us,
        nullptr
    };

    static Page page(items, nullptr, nullptr, FuncStartTest);
}


Page *PageSignal3b::self = &PageSignal3b::page;
