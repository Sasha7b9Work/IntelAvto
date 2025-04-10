// 2024/01/04 14:15:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/MenuItemsDef.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Display/Text_.h"
#include "Display/Display_.h"
#include "Device/IT6523.h"
#include "Device/Device.h"


namespace PageSignal5a
{
    VParameter param_Us("Us",
        &gset.signals[TypeSignal::_5a_16750_1].values12[0], Voltage(79), Voltage(101),
        &gset.signals[TypeSignal::_5a_16750_1].values24[0], Voltage(101), Voltage(202),
        355, 125);

    TParameter param_td("td",
        &gset.signals[TypeSignal::_5a_16750_1].values12[1], Time(40), Time(400),
        &gset.signals[TypeSignal::_5a_16750_1].values24[1], Time(100), Time(350),
        220, 155);

    CParameter param_Ri("Ri",
        &gset.signals[TypeSignal::_5b_16750_2].values12[3], Counter(1), Counter(10),
        &gset.signals[TypeSignal::_5b_16750_2].values24[3], Counter(1), Counter(10),
        90, Parameter::CalculateY(4));

    static CParameter param_N("N",
        &gset.signals[TypeSignal::_5a_16750_1].values12[2], Counter(1), Counter(1000),
        &gset.signals[TypeSignal::_5a_16750_1].values24[2], Counter(1), Counter(1000),
        90, Parameter::CalculateY(5));

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal5b::self);
        TypeSignal::Set(TypeSignal::_5b_16750_2);
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
        if (!IT6523::_Start(TypeSignal::_5a_16750_1, param_N.GetValue().ToInt()))
        {
            Display::ShowWarningMessage(WarningMessage::LittleTimeHeavyImpulse);
        }
    }

    static Item *items[] =
    {
        &bSignal5a,
        &chModeVoltage,
        &param_Us,
        &param_td,
        &param_Ri,
        &param_N,
        nullptr
    };

    static void AdditionDraw()
    {
        Text("%s     Импульсов: %d", VoltageMode::TextValue(),
            Device::IsRunning() ? IT6523::RemainedPulses() : param_N.GetValue().ToInt()).Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );

        Page::DrawConnectionServer();
    }

    static Page page(items, AdditionDraw, FuncStartTest);
}


Page *PageSignal5a::self = &PageSignal5a::page;
