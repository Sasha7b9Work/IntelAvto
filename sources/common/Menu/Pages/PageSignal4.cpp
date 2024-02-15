// 2024/01/04 14:11:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Connector/Messages.h"


namespace PageSignal4
{
    Parameter param_Us("Us", "Us",
        &gset.signals[TypeSignal::_4].values12[0], Value(0), Value(10000),
        &gset.signals[TypeSignal::_4].values24[0], Value(0), Value(10000),
        230, 110);

    Parameter param_Ua("Ua", "Ua",
        &gset.signals[TypeSignal::_4].values12[1], Value(0), Value(10000),
        &gset.signals[TypeSignal::_4].values24[1], Value(0), Value(10000),
        350, 100);

    Parameter param_t7("t7", "t7",
        &gset.signals[TypeSignal::_4].values12[2], Value(0), Value(10000),
        &gset.signals[TypeSignal::_4].values24[2], Value(0), Value(10000),
        220, 160);

    Parameter param_t9("t9", "t9",
        &gset.signals[TypeSignal::_4].values12[3], Value(0), Value(10000),
        &gset.signals[TypeSignal::_4].values24[3], Value(0), Value(10000),
        330, 160);

    Parameter param_t11("t11", "t11",
        &gset.signals[TypeSignal::_4].values12[4], Value(0), Value(10000),
        &gset.signals[TypeSignal::_4].values24[4], Value(0), Value(10000),
        410, 160);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal5a::self);
        TypeSignal::Set(TypeSignal::_5a);
    }

    DEF_BUTTON(bSignal5a,
        "Сигнал", "Signal",
        FuncPress_Signal
    );

    static void FuncStartTest()
    {
    }

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

    static Page page(items, nullptr, nullptr, FuncStartTest);
}


Page *PageSignal4::self = &PageSignal4::page;
