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


namespace PageSignal2a
{
    static VParameter param_Us("Us",
        &gset.signals[TypeSignal::_2a].values12[0], Voltage(37), Voltage(112),
        &gset.signals[TypeSignal::_2a].values24[0], Voltage(37), Voltage(112),
        320, 140);

    static TParameter param_t1("Период",
        &gset.signals[TypeSignal::_2a].values12[1], Time(200), Time(5000),
        &gset.signals[TypeSignal::_2a].values24[1], Time(200), Time(5000),
        270, 30);

    static CParameter param_N("N",
        &gset.signals[TypeSignal::_2a].values12[2], Counter(5000), Counter(100000),
        &gset.signals[TypeSignal::_2a].values24[2], Counter(5000), Counter(100000),
        90, 130);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal2b::self);
        TypeSignal::Set(TypeSignal::_2b_SAEJ1113);
    }

    DEF_BUTTON(bSignal2a,
        "Сигнал",
        FuncPress_Signal
    );

    DEF_CHOICE_2(chModeVoltage,
        "12В", "24В",
        gset.voltage_mode,
        FuncVV
    );

    void Start()
    {
        Message::Start2A(param_Us.GetValue(), param_t1.GetValue()).Transmit();

        RemainingTimeCounter::Start(param_t1, param_N);
    }

    static Item *items[] =
    {
        &bSignal2a,
        &chModeVoltage,
        &param_Us,
        &param_t1,
        &param_N,
        nullptr
    };

    static void AdditionDraw()
    {
        char buffer[128];

        Text("%s     Ri: 2 Ом     Длит: %s", VoltageMode::TextValue(), Duration(param_t1, param_N).ToStringValue(buffer)).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );
    }

    static Page page(items, AdditionDraw, PageSignal2a::Start);
}


Page *PageSignal2a::self = &PageSignal2a::page;
