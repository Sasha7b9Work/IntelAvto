// 2024/01/04 14:05:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Connector/Device/Messages_.h"
#include "Display/Text_.h"
#include "Display/Display_.h"
#include "Hardware/Timer.h"
#include "Device/IT6523.h"


namespace PageSignal3b
{
    static const int period_ms = 100;

    static VParameter param_Us("Us",
        &gset.signals[TypeSignal::_3b].values12[0], Voltage(us12_min * 1000), Voltage(us12_max * 1000),
        &gset.signals[TypeSignal::_3b].values24[0], Voltage(us24_min * 1000), Voltage(us24_max * 1000),
        250, 110);

    static AParameter param_A("Imax",
        &gset.signals[TypeSignal::_3b].values12[2], Current(Current::min * 1000), Current(Current::max * 1000),
        &gset.signals[TypeSignal::_3b].values24[2], Current(Current::min * 1000), Current(Current::max * 1000),
        Page::x_param, Item::Height() * 4 + Page::d_y);

    static CParameter param_N("N",
        &gset.signals[TypeSignal::_3b].values12[1], Counter(10000), Counter(100000),
        &gset.signals[TypeSignal::_3b].values24[1], Counter(10000), Counter(100000),
        90, 100);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal4::self);
        TypeSignal::Set(TypeSignal::_4_DIN40839);
    }

    DEF_BUTTON(bSignal3b,
        "������",
        FuncPress_Signal
    );

    DEF_CHOICE_2(chModeVoltage,
        "12�", "24�",
        gset.voltage_mode,
        FuncVV
    );

    static bool FuncStartTest()
    {
        Message::Start3B(param_Us.GetCalibrateValue(TypeSignal::_3b, VoltageMode::Current())).Transmit();

        RemainingTimeCounter::Start(period_ms, param_N);

        IT6523::Start(param_A.GetValue());

        return true;
    }

    static Item *items[] =
    {
        &bSignal3b,
        &chModeVoltage,
        &param_Us,
        &param_N,
        &param_A,
        nullptr
    };

    static void AdditionDraw()
    {
        char buffer[128];

        Text("%s     Ri: 50 ��     ����: %s", VoltageMode::TextValue(), Duration((float)period_ms / 1e3f, param_N).ToStringValue(buffer)).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );

        Page::DrawConnectionServer();
    }

    static Page page(items, AdditionDraw, FuncStartTest);
}


Page *PageSignal3b::self = &PageSignal3b::page;
