// 2024/01/04 14:18:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Display/Text_.h"
#include "Display/Display_.h"
#include "Device/IT6523.h"


namespace PageSignal5b
{
    static VParameter param_Us("Us",
        &gset.signals[TypeSignal::_5b_16750_2].values12[0], Voltage(79), Voltage(101),
        &gset.signals[TypeSignal::_5b_16750_2].values24[0], Voltage(101), Voltage(202),
        420, 130);

    static TParameter param_td("td",
        &gset.signa ls[TypeSignal::_5b_16750_2].values12[2], Time(40), Time(400),
        &gset.signals[TypeSignal::_5b_16750_2].values24[2], Time(100), Time(350),
        220, 160);

    static CParameter param_N("N",
        &gset.signals[TypeSignal::_5b_16750_2].values12[3], Counter(1), Counter(1000),
        &gset.signals[TypeSignal::_5b_16750_2].values24[3], Counter(1), Counter(1000),
        90, 70);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal1::self);
        TypeSignal::Set(TypeSignal::_1);
    }

    DEF_BUTTON(bSignal5b,
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
        IT6523::Start(TypeSignal::_5b_16750_2, param_N.GetValue().ToInt());
    }

    static Item *items[] =
    {
        &bSignal5b,
        &chModeVoltage,
        &param_N,
        &param_Us,
        &param_td,
        nullptr
    };

    static void AdditionDraw()
    {
        Text("%s     Us*: %d В", VoltageMode::TextValue(), VoltageMode::Is12() ? 35 : 59).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );

        Page::DrawConnectionServer();
    }

    static Page page(items, AdditionDraw, FuncStartTest);
}


Page *PageSignal5b::self = &PageSignal5b::page;
