// 2024/01/04 13:56:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Menu/MenuItemsDef.h"
#include "Connector/Messages.h"


namespace PageSignal3a
{
    Parameter param_Us("Us", "Us",
        &gset.signals[TypeSignal::_3a].values12[0], Value(0), Value(10000),
        &gset.signals[TypeSignal::_3b].values24[0], Value(0), Value(10000),
        275, 95);

    Parameter param_duration("����", "Duration",
        &gset.signals[TypeSignal::_3a].values12[1], Value(200), Value(5000),
        &gset.signals[TypeSignal::_3a].values24[1], Value(200), Value(5000),
        90, 50);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal3b::self);
        TypeSignal::Set(TypeSignal::_3b);
    }

    DEF_BUTTON(bSignal3a,
        "������", "Signal",
        FuncPress_Signal
    );

    static void FuncStartTest()
    {
        Message::Start3A(param_Us.GetValue(), param_duration.GetValue()).Transmit();
    }

    static Item *items[] =
    {
        &bSignal3a,
        &param_Us,
        &param_duration,
        nullptr
    };

    static Page page(items, nullptr, nullptr, FuncStartTest);
}


Page *PageSignal3a::self = &PageSignal3a::page;
