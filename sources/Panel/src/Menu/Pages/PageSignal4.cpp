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

    static VParameter param_Us("Us",
        &gset.signals[TypeSignal::_4_DIN40839].values12[1], Voltage(0), Voltage(10000),
        &gset.signals[TypeSignal::_4_DIN40839].values24[1], Voltage(0), Voltage(10000),
        230, 110);

    static VParameter param_Ua("Ua",
        &gset.signals[TypeSignal::_4_DIN40839].values12[2], Voltage(0), Voltage(10000),
        &gset.signals[TypeSignal::_4_DIN40839].values24[2], Voltage(0), Voltage(10000),
        350, 100);

    static TParameter param_t7("t7",
        &gset.signals[TypeSignal::_4_DIN40839].values12[3], Time(0), Time(10000),
        &gset.signals[TypeSignal::_4_DIN40839].values24[3], Time(0), Time(10000),
        220, 160);

    static TParameter param_t9("t9",
        &gset.signals[TypeSignal::_4_DIN40839].values12[4], Time(0), Time(10000),
        &gset.signals[TypeSignal::_4_DIN40839].values24[4], Time(0), Time(10000),
        330, 160);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal5a::self);
        TypeSignal::Set(TypeSignal::_5a_16750_1);
    }

    DEF_BUTTON(bSignal5a,
        "������",
        FuncPress_Signal
    );

    DEF_CHOICE_2(chModeVoltage,
        "12�", "24�",
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
            VoltageMode::Is12() ? "-7.5 �" : "-16 �",
            VoltageMode::Is12() ? "-6 �" : "-12 �",
            VoltageMode::Is12() ? "15 ��" : "50 ��",
            "2000 ��"
        ).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );

        Page::DrawConnectionServer();
    }

    static Page page(items, AdditionDraw, FuncStartTest);
}


Page *PageSignal4::self = &PageSignal4::page;
