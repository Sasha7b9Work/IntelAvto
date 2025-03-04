// 2024/01/04 14:11:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Display/Text_.h"
#include "Display/Display_.h"


namespace PageSignal4
{
    static VParameter param_Us("Us",
        &gset.signals[TypeSignal::_4].values12[0], Voltage(0), Voltage(10000),
        &gset.signals[TypeSignal::_4].values24[0], Voltage(0), Voltage(10000),
        230, 110);

    static VParameter param_Ua("Ua",
        &gset.signals[TypeSignal::_4].values12[1], Voltage(0), Voltage(10000),
        &gset.signals[TypeSignal::_4].values24[1], Voltage(0), Voltage(10000),
        350, 100);

    static TParameter param_t7("t7",
        &gset.signals[TypeSignal::_4].values12[2], Time(0), Time(10000),
        &gset.signals[TypeSignal::_4].values24[2], Time(0), Time(10000),
        220, 160);

    static TParameter param_t9("t9",
        &gset.signals[TypeSignal::_4].values12[3], Time(0), Time(10000),
        &gset.signals[TypeSignal::_4].values24[3], Time(0), Time(10000),
        330, 160);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal5a::self);
        TypeSignal::Set(TypeSignal::_5a);
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
    }

    static Item *items[] =
    {
        &bSignal5a,
        &chModeVoltage,
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
