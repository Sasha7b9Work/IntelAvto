// 2024/01/04 14:05:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Connector/Messages_.h"
#include "Display/Text.h"
#include "Display/Display.h"


namespace PageSignal3b
{
    Parameter param_Us("Us",
        &gset.signals[TypeSignal::_3b].values12[0], Value(0), Value(10000),
        &gset.signals[TypeSignal::_3b].values24[0], Value(0), Value(10000),
        350, 70);

    Parameter param_duration("Длит",
        &gset.signals[TypeSignal::_3b].values12[1], Value(200), Value(5000),
        &gset.signals[TypeSignal::_3b].values24[1], Value(200), Value(5000),
        90, 50);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal4::self);
        TypeSignal::Set(TypeSignal::_4);
    }

    DEF_BUTTON(bSignal3b,
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
        Message::Start3B(param_Us.GetValue(), param_duration.GetValue()).Transmit();
    }

    static Item *items[] =
    {
        &bSignal3b,
        &chModeVoltage,
        &param_Us,
        &param_duration,
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


Page *PageSignal3b::self = &PageSignal3b::page;
