// 2024/01/04 11:55:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Display/Text_.h"
#include "Display/Display_.h"
#include "LAN/ServerTCP.h"
#include "Device/IT6523.h"


namespace PageSignal2b
{
    static TParameter param_td("td",
        &gset.signals[TypeSignal::_2b].values12[0], Time(200), Time(2000),
        &gset.signals[TypeSignal::_2b].values24[0], Time(200), Time(2000),
        290, 60);

    static CParameter param_N("N",
        &gset.signals[TypeSignal::_2b].values12[1], Counter(1), Counter(1000),
        &gset.signals[TypeSignal::_2b].values24[1], Counter(1), Counter(1000),
        90, 130
    );

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal3a::self);
        TypeSignal::Set(TypeSignal::_3a);
    }

    DEF_BUTTON(bSignal2b,
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
        IT6523::SendCommand("carwave:sae:2b:volt 12V");
        IT6523::SendCommand("carwave:sae:2b:td 0.2");
        IT6523::SendCommand("carwave:sae:2b:state 1");
        IT6523::Start();
    }

    static Item *items[] =
    {
        &bSignal2b,
        &chModeVoltage,
        &param_td,
        &param_N,
        nullptr
    };

    static void AdditionDraw()
    {
        Text("%s     Ri: 0.05 Ом", VoltageMode::TextValue()).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );

        Page::DrawConnectionServer();
    }

    static Page page(items, AdditionDraw, FuncStartTest);
}


Page *PageSignal2b::self = &PageSignal2b::page;
