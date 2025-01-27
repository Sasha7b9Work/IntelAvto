// 2024/01/04 13:56:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Menu/MenuItemsDef.h"
#include "Connector/Device/Messages_.h"
#include "Display/Text_.h"
#include "Display/Display_.h"


namespace PageSignal3a
{
    Parameter param_Us("Us",
        &gset.signals[TypeSignal::_3a].values12[0], Value(-112, TypeValue::Voltage), Value(-150, TypeValue::Voltage),
        &gset.signals[TypeSignal::_3a].values24[0], Value(-150, TypeValue::Voltage), Value(-200, TypeValue::Voltage),
        275, 95);

    Parameter param_N("N",
        &gset.signals[TypeSignal::_3a].values12[1], Value(10000), Value(100000),
        &gset.signals[TypeSignal::_3a].values24[1], Value(10000), Value(100000),
        90, 100);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal3b::self);
        TypeSignal::Set(TypeSignal::_3b);
    }

    DEF_BUTTON(bSignal3a,
        "Сигнал",
        FuncPress_Signal
    );

    DEF_CHOICE_2(chModeVoltage,
        "12V", "24V",
        gset.voltage_mode,
        FuncVV
    );

    static void FuncStartTest()
    {
        Message::Start3A(param_Us.GetValue()).Transmit();
    }

    static Item *items[] =
    {
        &bSignal3a,
        &chModeVoltage,
        &param_Us,
        &param_N,
        nullptr
    };

    static void AdditionDraw()
    {
        Text("Ri : 50 Ом").Write(
            Display::xConstParameters, Display::yConstParameters
        );
    }

    static Page page(items, nullptr, AdditionDraw, FuncStartTest);
}


Page *PageSignal3a::self = &PageSignal3a::page;
