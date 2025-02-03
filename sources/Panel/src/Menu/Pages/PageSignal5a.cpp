// 2024/01/04 14:15:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Display/Text_.h"
#include "Display/Display_.h"


namespace PageSignal5a
{
    VParameter param_Us("Us",
        &gset.signals[TypeSignal::_5a].values12[0], Voltage(0), Voltage(10000),
        &gset.signals[TypeSignal::_5a].values24[0], Voltage(0), Voltage(10000),
        355, 125);

    TParameter param_td("td",
        &gset.signals[TypeSignal::_5a].values12[1], Time(0), Time(10000),
        &gset.signals[TypeSignal::_5a].values24[1], Time(0), Time(10000),
        220, 155);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal5b::self);
        TypeSignal::Set(TypeSignal::_5b);
    }

    DEF_BUTTON(bSignal5a,
        "Сигнал",
        FuncPress_Signal
    );

    DEF_CHOICE_2(chModeVoltage,
        "12В", "24В",
        gset.voltage_mode,
        FuncVV
    );

    static void FuncStartTest()
    {
    }

    static Item *items[] =
    {
        &bSignal5a,
        &chModeVoltage,
        &param_Us,
        &param_td,
        nullptr
    };

    static void AdditionDraw()
    {
        Text("%s", VoltageMode::TextValue()).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );
    }

    static Page page(items, AdditionDraw, FuncStartTest);
}


Page *PageSignal5a::self = &PageSignal5a::page;
