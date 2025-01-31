// 2023/12/25 17:20:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include "Connector/Device/Messages_.h"
#include "Display/Text_.h"
#include "Display/Display_.h"
#include "Hardware/Timer.h"


namespace PageSignal1
{
    // ���������
    Parameter param_Us("Us",
        &gset.signals[TypeSignal::_1].values12[0], Value(-600, TypeValue::Voltage), Value(-1, TypeValue::Voltage),
        &gset.signals[TypeSignal::_1].values24[0], Value(-600, TypeValue::Voltage), Value(-1, TypeValue::Voltage),
        280, 155);

    // ������ ����������
    Parameter param_t1("������",
        &gset.signals[TypeSignal::_1].values12[1], Value(500), Value(5000),
        &gset.signals[TypeSignal::_1].values24[1], Value(500), Value(5000),
        280, 50);

    // ���������� ���������
    Parameter param_N("N",
        &gset.signals[TypeSignal::_1].values12[2], Value(5000), Value(100000),
        &gset.signals[TypeSignal::_1].values24[2], Value(5000), Value(100000),
        90, 130);

    static void FuncPress_Signal()
    {
        Menu::SetOpenedPage(PageSignal2a::self);

        TypeSignal::Set(TypeSignal::_2a);
    }

    DEF_BUTTON(bSignal1,
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
        if (VoltageMode::Is12())
        {
            Message::Start1_12V(param_Us.GetValue(), param_t1.GetValue()).Transmit();
        }
        else
        {
            Message::Start1_24V(param_Us.GetValue(), param_t1.GetValue()).Transmit();
        }

        RemainingTimeCounter::Start(param_t1, param_N);
    }


    static Item *items[] =
    {
        &bSignal1,
        &chModeVoltage,
        &param_Us,
        &param_t1,
        &param_N,
        nullptr
    };

    static void AdditionDraw()
    {
        char buffer[32];

        Text("%s     ����: %s     Ri: %s", VoltageMode::TextValue(), Duration(param_t1, param_N).ToStringValue(buffer), VoltageMode::Is12() ? "10 ��" : "50 ��").Write(
            Display::xConstParameters, Display::yConstParameters, Color::WHITE
        );
    }

    static Page page(items, nullptr, AdditionDraw, FuncStartTest);
}


Page *PageSignal1::self = &PageSignal1::page;
