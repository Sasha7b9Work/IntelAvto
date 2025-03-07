// 2024/01/04 14:18:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Display/Text_.h"
#include "Display/Display_.h"


namespace PageSignal5b
{
    static VParameter param_Us("Us",
        &gset.signals[TypeSignal::_5b].values12[0], Voltage(0), Voltage(10000),
        &gset.signals[TypeSignal::_5b].values24[0], Voltage(0), Voltage(10000),
        420, 130);

    static VParameter param_Uss("Us*",
        &gset.signals[TypeSignal::_5b].values12[1], Voltage(0), Voltage(10000),
        &gset.signals[TypeSignal::_5b].values24[1], Voltage(0), Voltage(10000),
        380, 160);

    static TParameter param_td("td",
        &gset.signals[TypeSignal::_5b].values12[2], Time(0), Time(10000),
        &gset.signals[TypeSignal::_5b].values24[2], Time(0), Time(10000),
        220, 160);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal1::self);
        TypeSignal::Set(TypeSignal::_1);
    }

    DEF_BUTTON(bSignal5b,
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
        &bSignal5b,
        &chModeVoltage,
        &param_Us,
        &param_Uss,
        &param_td,
        nullptr
    };

    static void AdditionDraw()
    {
        Text("%s     Us*: %d �", VoltageMode::TextValue(), VoltageMode::Is12() ? 35 : 59).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );

        Page::DrawConnectionServer();
    }

    static Page page(items, AdditionDraw, FuncStartTest);
}


Page *PageSignal5b::self = &PageSignal5b::page;
