// 2024/01/04 11:55:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Display/Text_.h"
#include "Display/Display_.h"


namespace PageSignal2b
{
    VParameter param_Us("Us",
        &gset.signals[TypeSignal::_2b].values12[0], Voltage(0), Voltage(10000),
        &gset.signals[TypeSignal::_2b].values24[0], Voltage(0), Voltage(10000),
        430, 170);

    TParameter param_td("td",
        &gset.signals[TypeSignal::_2b].values12[1], Time(0), Time(10000),
        &gset.signals[TypeSignal::_2b].values24[2], Time(0), Time(10000),
        290, 60);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal3a::self);
        TypeSignal::Set(TypeSignal::_3a);
    }

    DEF_BUTTON(bSignal2b,
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
        &bSignal2b,
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

        Page::DrawConnectionServer();
    }

    static Page page(items, AdditionDraw, FuncStartTest);
}


Page *PageSignal2b::self = &PageSignal2b::page;
