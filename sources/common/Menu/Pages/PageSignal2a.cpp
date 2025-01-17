// 2024/01/04 11:26:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Connector/Messages_.h"
#include "Display/Text.h"
#include "Display/Display.h"


namespace PageSignal2a
{
    Parameter param_Us("Us",
        &gset.signals[TypeSignal::_2a].values12[0], Value(37), Value(112),
        &gset.signals[TypeSignal::_2a].values24[0], Value(37), Value(112),
        320, 140);

    Parameter param_period("Период",
        &gset.signals[TypeSignal::_2a].values12[1], Value(200), Value(5000),
        &gset.signals[TypeSignal::_2a].values24[1], Value(200), Value(5000),
        270, 30);

    Parameter param_duration("Длит",
        &gset.signals[TypeSignal::_2a].values12[2], Value(1), Value(20),
        &gset.signals[TypeSignal::_2a].values24[2], Value(1), Value(20),
        90, 70);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal2b::self);
        TypeSignal::Set(TypeSignal::_2b);
    }

    DEF_BUTTON(bSignal2a,
        "Сигнал",
        FuncPress_Signal
    );

    DEF_CHOICE_2(chModeVoltage,
        "12V", "24V",
        gset.voltage_mode,
        FuncVV
    );

    void Start()
    {
        Message::Start2A(param_Us.GetValue(), param_period.GetValue()).Transmit();
    }

    static Item *items[] =
    {
        &bSignal2a,
        &chModeVoltage,
        &param_period,
        &param_duration,
        nullptr
    };

    static void AdditionDraw()
    {
        Text("Ri : 2 Ом").Write(
            Display::xConstParameters, Display::yConstParameters
        );
    }

    static Page page(items, nullptr, AdditionDraw, PageSignal2a::Start);
}


Page *PageSignal2a::self = &PageSignal2a::page;
