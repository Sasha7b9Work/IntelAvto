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
    static VParameter param_Us("Us",
        &gset.signals[TypeSignal::_5a_16750_1].values12[0], Voltage(0), Voltage(10000),
        &gset.signals[TypeSignal::_5a_16750_1].values24[0], Voltage(0), Voltage(10000),
        355, 125);

    static TParameter param_td("td",
        &gset.signals[TypeSignal::_5a_16750_1].values12[1], Time(0), Time(10000),
        &gset.signals[TypeSignal::_5a_16750_1].values24[1], Time(0), Time(10000),
        220, 155);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal5b::self);
        TypeSignal::Set(TypeSignal::_5b_16750_2);
    }

    DEF_BUTTON(bSignal5a,
        "������",
        FuncPress_Signal
    );

    DEF_CHOICE_2(chModeVoltage,
        "12�", "24�",
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

        Page::DrawConnectionServer();
    }

    static Page page(items, AdditionDraw, FuncStartTest);
}


Page *PageSignal5a::self = &PageSignal5a::page;
