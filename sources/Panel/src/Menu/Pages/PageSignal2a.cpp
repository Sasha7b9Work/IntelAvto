// 2024/01/04 11:26:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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


namespace PageSignal2a
{
    static VParameter param_Us("Us",
        &gset.signals[TypeSignal::_2a].values12[0], Voltage(us12_min * 1000), Voltage(us12_max * 1000),
        &gset.signals[TypeSignal::_2a].values24[0], Voltage(us24_min * 1000), Voltage(us24_max * 1000),
        350, 120);

    static AParameter param_A("Imax",
        &gset.signals[TypeSignal::_2a].values12[3], Current(Current::min * 1000), Current(Current::max * 1000),
        &gset.signals[TypeSignal::_2a].values24[3], Current(Current::min * 1000), Current(Current::max * 1000),
        Page::x_param, Item::Height() * 5 + Page::d_y);

    static TParameter param_t1("������",
        &gset.signals[TypeSignal::_2a].values12[1], Time(200), Time(5000),
        &gset.signals[TypeSignal::_2a].values24[1], Time(200), Time(5000),
        250, 70);

    static CParameter param_N("N",
        &gset.signals[TypeSignal::_2a].values12[2], Counter(5000), Counter(100000),
        &gset.signals[TypeSignal::_2a].values24[2], Counter(5000), Counter(100000),
        Page::x_param, Item::Height() * 4 + Page::d_y);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal2b::self);
        TypeSignal::Set(TypeSignal::_2b_SAEJ1113);
    }

    DEF_BUTTON(bSignal2a,
        "������",
        FuncPress_Signal
    );

    DEF_CHOICE_2(chModeVoltage,
        "12�", "24�",
        gset.voltage_mode,
        FuncVV
    );

    bool Start()
    {
        Message::Start2A(param_Us.GetCalibrateValue(TypeSignal::_2a, VoltageMode::Current()), param_t1.GetValue()).Transmit();

        RemainingTimeCounter::Start(param_t1.GetValue().ToMU(), param_N);

        IT6523::Start(param_A.GetValue());

        return true;
    }

    static Item *items[] =
    {
        &bSignal2a,
        &chModeVoltage,
        &param_Us,
        &param_t1,
        &param_N,
        &param_A,
        nullptr
    };

    static void AdditionDraw()
    {
        char buffer[128];

        Text("%s     Ri: 2 ��     ����: %s", VoltageMode::TextValue(), Duration(param_t1, param_N).ToStringValue(buffer)).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );

        Page::DrawConnectionServer();
    }

    static Page page(items, AdditionDraw, PageSignal2a::Start);
}


Page *PageSignal2a::self = &PageSignal2a::page;
