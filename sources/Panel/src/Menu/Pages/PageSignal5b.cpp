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
    Parameter param_Us("Us",
        &gset.signals[TypeSignal::_5b].values12[0], Value(0, TypeValue::Voltage), Value(10000, TypeValue::Voltage),
        &gset.signals[TypeSignal::_5b].values24[0], Value(0, TypeValue::Voltage), Value(10000, TypeValue::Voltage),
        420, 130);

    Parameter param_Uss("Us*",
        &gset.signals[TypeSignal::_5b].values12[1], Value(0, TypeValue::Voltage), Value(10000, TypeValue::Voltage),
        &gset.signals[TypeSignal::_5b].values24[1], Value(0, TypeValue::Voltage), Value(10000, TypeValue::Voltage),
        380, 160);

    Parameter param_td("td",
        &gset.signals[TypeSignal::_5b].values12[2], Value(0, TypeValue::Time), Value(10000, TypeValue::Time),
        &gset.signals[TypeSignal::_5b].values24[2], Value(0, TypeValue::Time), Value(10000, TypeValue::Time),
        220, 160);

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
        Text("%s     Us*: %d В", VoltageMode::TextValue(), VoltageMode::Is12() ? 35 : 59).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );
    }

    static Page page(items, nullptr, AdditionDraw, FuncStartTest);
}


Page *PageSignal5b::self = &PageSignal5b::page;
