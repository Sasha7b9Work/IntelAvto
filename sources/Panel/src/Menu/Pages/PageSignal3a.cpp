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
        &gset.signals[TypeSignal::_3a].values12[0], Value(0), Value(10000),
        &gset.signals[TypeSignal::_3b].values24[0], Value(0), Value(10000),
        275, 95);

    Parameter param_duration("Длит",
        &gset.signals[TypeSignal::_3a].values12[1], Value(200), Value(5000),
        &gset.signals[TypeSignal::_3a].values24[1], Value(200), Value(5000),
        90, 50);

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


Page *PageSignal3a::self = &PageSignal3a::page;
