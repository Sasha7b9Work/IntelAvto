// 2023/12/25 17:20:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Connector/Messages_.h"
#include "Display/Text.h"
#include "Display/Display.h"


namespace PageSignal1
{
    Parameter param_Us("Us",
        &gset.signals[TypeSignal::_1].values12[0], Value(0, TypeValue::Volts), Value(800, TypeValue::Volts),
        &gset.signals[TypeSignal::_1].values24[0], Value(0, TypeValue::Volts), Value(800, TypeValue::Volts),
        340, 165);

    Parameter param_period("ѕериод",
        &gset.signals[TypeSignal::_1].values12[1], Value(500), Value(5000),
        &gset.signals[TypeSignal::_1].values24[1], Value(500), Value(5000),
        310, 33);

    Parameter param_N("N",
        &gset.signals[TypeSignal::_1].values12[2], Value(5000), Value(100000),
        &gset.signals[TypeSignal::_1].values24[2], Value(5000), Value(100000),
        90, 100);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal2a::self);

        TypeSignal::Set(TypeSignal::_2a);
    }


    DEF_BUTTON(bSignal1,
        "—игнал",
        FuncPress_Signal
    );

    DEF_CHOICE_2(chModeVoltage,
        "12V", "24V",
        gset.voltage_mode,
        FuncVV
    );

    static void FuncStartTest()
    {
        Message::Start1(param_Us.GetValue(), param_period.GetValue(), Value(1000U)).Transmit();
    }


    static Item *items[] =
    {
        &bSignal1,
        &chModeVoltage,
        &param_Us,
        &param_period,
        &param_N,
        nullptr
    };

    static void AdditionDraw()
    {
        Text("ƒлительность : 750 мс     Ri : %s", VoltageMode::Is12() ? "10 ќм" : "50 ќм").Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );
    }

    static Page page(items, nullptr, AdditionDraw, FuncStartTest);
}


Page *PageSignal1::self = &PageSignal1::page;
