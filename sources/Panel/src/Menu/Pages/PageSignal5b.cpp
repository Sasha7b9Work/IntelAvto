// 2024/01/04 14:18:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Display/Text_.h"
#include "Display/Display_.h"
#include "Device/IT6523.h"
#include "Device/Device.h"


namespace PageSignal5b
{
    VParameter param_Us("Us",
        &gset.signals[TypeSignal::_5b_16750_2].values12[0], Voltage(79), Voltage(101),
        &gset.signals[TypeSignal::_5b_16750_2].values24[0], Voltage(101), Voltage(202),
        420, 130);

    TParameter param_td("td",
        &gset.signals[TypeSignal::_5b_16750_2].values12[2], Time(40), Time(400),
        &gset.signals[TypeSignal::_5b_16750_2].values24[2], Time(100), Time(350),
        220, 160);

    CParameter param_Ri("Ri",
        &gset.signals[TypeSignal::_5b_16750_2].values12[1], Counter(1), Counter(10),
        &gset.signals[TypeSignal::_5b_16750_2].values24[1], Counter(1), Counter(10),
        90, Parameter::CalculateY(4));

    static CParameter param_N("N",
        &gset.signals[TypeSignal::_5b_16750_2].values12[3], Counter(1), Counter(1000),
        &gset.signals[TypeSignal::_5b_16750_2].values24[3], Counter(1), Counter(1000),
        90, Parameter::CalculateY(5));

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal1::self);
        TypeSignal::Set(TypeSignal::_1);
    }

    DEF_BUTTON(bSignal5b,
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
        if (!IT6523::_Start(TypeSignal::_5b_16750_2, param_N.GetValue().ToInt()))
        {
            Display::ShowWarningMessage(WarningMessage::LittleTimeHeavyImpulse);
        }
    }

    static Item *items[] =
    {
        &bSignal5b,
        &chModeVoltage,
        &param_Us,
        &param_td,
        &param_Ri,
        &param_N,
        nullptr
    };

    static void AdditionDraw()
    {
        Text("%s     Us*: %d �     ���������: %d",
            VoltageMode::TextValue(), VoltageMode::Is12() ? 35 : 59, Device::IsRunning() ? IT6523::RemainedPulses() : param_N.GetValue().ToInt()).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );

        Page::DrawConnectionServer();
    }

    static Page page(items, AdditionDraw, FuncStartTest);
}


Page *PageSignal5b::self = &PageSignal5b::page;
