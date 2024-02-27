// 2023/12/25 17:20:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Connector/Messages.h"


namespace PageSignal1
{
    Parameter param_Us("Us", "Us",
        &gset.signals[TypeSignal::_1].values12[0], Value(75, Volts), Value(150, Volts),
        &gset.signals[TypeSignal::_1].values24[0], Value(30, Volts), Value(60, Volts),
        340, 165);

    Parameter param_period("Период", "Period",
        &gset.signals[TypeSignal::_1].values12[1], Value(500), Value(5000),
        &gset.signals[TypeSignal::_1].values24[1], Value(500), Value(5000),
        310, 33);

    Parameter param_N("N", "N",
        &gset.signals[TypeSignal::_1].values12[2], Value(5000), Value(100000),
        &gset.signals[TypeSignal::_1].values24[2], Value(5000), Value(100000),
        90, 100);

    Parameter param_duration("Длит", "Duration",
        &gset.signals[TypeSignal::_1].values12[3], Value(1), Value(20),
        &gset.signals[TypeSignal::_1].values24[3], Value(1), Value(20),
        90, 70);

    Parameter param_t2("t2", "t2",
        &gset.signals[TypeSignal::_1].values12[4], Value(100), Value(200),
        &gset.signals[TypeSignal::_1].values24[4], Value(100), Value(200),
        90, 160);

    Parameter param_td("td", "td",
        &gset.signals[TypeSignal::_1].values12[5], Value(1), Value(100),
        &gset.signals[TypeSignal::_1].values24[5], Value(1), Value(100),
        90, 190);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal2a::self);

        TypeSignal::Set(TypeSignal::_2a);
    }


    DEF_BUTTON(bSignal1,
        "Сигнал", "Signal",
        FuncPress_Signal
    );

    static void FuncStartTest()
    {
        Message::Start1(param_Us.GetValue(), param_period.GetValue(), param_duration.GetValue()).Transmit();
    }


    static Item *items[] =
    {
        &bSignal1,
        &param_Us,
        &param_period,
//        &param_N,
        &param_duration,
//        &param_t2,
//        &param_td,
        nullptr
    };

    static Page page(items, nullptr, nullptr, FuncStartTest);
}


Page *PageSignal1::self = &PageSignal1::page;
