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
    static VParameter param_Us("Us",
        &gset.signals[TypeSignal::_3a].values12[0], Voltage(112000), Voltage(150000),
        &gset.signals[TypeSignal::_3a].values24[0], Voltage(150000), Voltage(200000),
        275, 95);

    static CParameter param_N("N",
        &gset.signals[TypeSignal::_3a].values12[1], Counter(10000), Counter(100000),
        &gset.signals[TypeSignal::_3a].values24[1], Counter(10000), Counter(100000),
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
        "12В", "24В",
        gset.voltage_mode,
        FuncVV
    );

    static bool FuncStartTest()
    {
        Message::Start3A(param_Us.GetCalibrateValue(TypeSignal::_3a, VoltageMode::Current())).Transmit();

        return true;
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
        char buffer[128];

        Text("%s     Ri: 50 Ом     Длит: %s", VoltageMode::TextValue(), Duration(1.0f, param_N).ToStringValue(buffer)).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );

        Page::DrawConnectionServer();
    }

    static Page page(items, AdditionDraw, FuncStartTest);
}


Page *PageSignal3a::self = &PageSignal3a::page;
