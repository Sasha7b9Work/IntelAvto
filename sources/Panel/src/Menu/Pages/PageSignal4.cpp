// 2024/01/04 14:11:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Display/Text_.h"
#include "Display/Display_.h"
#include "Device/IT6523.h"


namespace PageSignal4
{
    static CParameter param_N("N",
        &gset.signals[TypeSignal::_4_DIN40839].values12[0], Counter(1), Counter(1000),
        &gset.signals[TypeSignal::_4_DIN40839].values24[0], Counter(1), Counter(1000),
        90, 70);

    VParameter param_Us("Us",
        &gset.signals[TypeSignal::_4_DIN40839].values12[1], Voltage(-6000), Voltage(-7500),
        &gset.signals[TypeSignal::_4_DIN40839].values24[1], Voltage(-12000), Voltage(-16000),
        230, 110);

    VParameter param_Ua("Ua",
        &gset.signals[TypeSignal::_4_DIN40839].values12[2], Voltage(-2500), Voltage(-6000),
        &gset.signals[TypeSignal::_4_DIN40839].values24[2], Voltage(-5000), Voltage(-12000),
        350, 100);

    TParameter param_t7("t7",
        &gset.signals[TypeSignal::_4_DIN40839].values12[3], Time(15), Time(40),
        &gset.signals[TypeSignal::_4_DIN40839].values24[3], Time(50), Time(100),
        220, 160);

    TParameter param_t9("t9",
        &gset.signals[TypeSignal::_4_DIN40839].values12[4], Time(500), Time(20000),
        &gset.signals[TypeSignal::_4_DIN40839].values24[4], Time(500), Time(20000),
        330, 160);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal5a::self);
        TypeSignal::Set(TypeSignal::_5a_16750_1);
    }

    DEF_BUTTON(bSignal5a,
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
        IT6523::Start(TypeSignal::_4_DIN40839, param_N.GetValue().ToInt());
    }

    static Item *items[] =
    {
        &bSignal5a,
        &chModeVoltage,
        &param_N,
        &param_Us,
        &param_Ua,
        &param_t7,
        &param_t9,
        nullptr
    };

    static void AdditionDraw()
    {
        Text("%s    Us: %s    Ua: %s    t7: %s    t9: %s",
            VoltageMode::TextValue(),
            VoltageMode::Is12() ? "-7.5 В" : "-16 В",
            VoltageMode::Is12() ? "-6 В" : "-12 В",
            VoltageMode::Is12() ? "15 мс" : "50 мс",
            "2000 мс"
        ).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );

        Page::DrawConnectionServer();
    }

    static Page page(items, AdditionDraw, FuncStartTest);
}


Page *PageSignal4::self = &PageSignal4::page;
