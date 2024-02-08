// 2024/01/04 11:26:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Connector/Messages.h"


namespace PageSignal2a
{
    Parameter param_Us("Us", "Us",
        &gset.signals[TypeSignal::_2a].values12[0], Value(37), Value(112),
        &gset.signals[TypeSignal::_2a].values24[0], Value(37), Value(112),
        320, 140);

    Parameter param_t1("t1", "t1",
        &gset.signals[TypeSignal::_2a].values12[1], Value(200), Value(5000),
        &gset.signals[TypeSignal::_2a].values24[1], Value(200), Value(5000),
        270, 30);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal2b::self);
        TypeSignal::Set(TypeSignal::_2b);
    }

    DEF_BUTTON(bSignal2a,
        "Сигнал", "Signal",
        FuncPress_Signal
    );

    void PageSignal2a::Start()
    {
        Message::Start2A(param_Us.GetValue(), param_t1.GetValue()).Transmit();
    }

    static Item *items[] =
    {
        &bSignal2a,
        &param_Us,
        &param_t1,
        nullptr
    };

    static Page page(items, nullptr, nullptr, PageSignal2a::Start);
}


Page *PageSignal2a::self = &PageSignal2a::page;
