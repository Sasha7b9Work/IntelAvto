// 2025/03/25 09:30:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Display/Text_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "Display/Display_.h"
#include "Hardware/Timer.h"


using namespace Primitives;


namespace PageCalibration
{
    static const int INVALID_VOLTAGE = 999999999;
    static const uint TIME_TIMER = 1500;

    static uint8 type_signal = 0;
    static uint8 type_accum = 0;
    static const int NUM_POINTS = 4;        // ������� ����� ����� ��� ����������
    static int  current_point = 0;          // �����, ������� ������ ��������
    static bool output_en = false;

    static int GetDisplayVoltage();

    static void EnableOutput();

    static void DisableOutput();

    // ��������� ������������� �����������
    static void SaveCalibrationFactor();

    // dir = 1/-1 �������� ������������� ����������� � ������� (1) ��� ������� (-1) �������
    static void ChangeCalibrationFactor(int dir);

    static void TimerFunction();

    struct State
    {
        enum E
        {
            Normal,             // ��������� ���������
            FactorSave,         // ������������� ����������� �������
            FactroNotSave       // ������������� ����������� �� �������
        };
    };

    static State::E state = State::Normal;

    static void FuncPress_Back()
    {
        Menu::SetOpenedPage(PageMain::self);
    }

    DEF_BUTTON(bBack,
        "�����",
        FuncPress_Back
    );

    DEF_CHOICE_4_FULL(chTypeSignal,
        "�������",
        "1", "2a", "3a", "3b",
        type_signal,
        FuncVV
    );

    DEF_CHOICE_2_FULL(chTypeAccum,
        "�����������",
        "12�", "24�",
        type_accum,
        FuncVV
    );

    static void FuncPress_Start()
    {

    }

    DEF_BUTTON(bStart,
        "�����������",
        FuncPress_Start
    );

    static void FuncPress_Reset()
    {
        
    }

    DEF_BUTTON(bReset,
        "�����",
        FuncPress_Reset
    );

    static Item *items[] =
    {
        &bBack,
        &chTypeSignal,
        &chTypeAccum,
        &bStart,
//        &bReset,
        nullptr
    };

    static void FuncDraw()
    {
        int dy = 25;

        if (state == State::Normal)
        {
            int x = 180;
            int y = 70;

            Text("��� ������ ����������").Write(x + 30, y, Color::WHITE);                    y += 2 * dy;

            Text("������� ������ \"�����������\".").Write(x, y);
        }
        else if (state == State::FactorSave)
        {
            int x = 180;
            int y = 70;

            Text("�����������").Write(x, y, Display::Width() - x, Color::GREEN); y += 30;

            Text("c�������").Write(x, y, Display::Width() - x);
        }
        else if (state == State::FactroNotSave)
        {
            int x = 180;
            int y = 70;

            Text("�����������").Write(x, y, Display::Width() - x, Color::RED); y += 30;

            Text("�� c�������").Write(x, y, Display::Width() - x);
        }

        Font::Set(TypeFont::GOSTB28B);

        Text("�����:").Write(30, 150);

        if (output_en)
        {
            Text("���").Write(45, 205, Color::GREEN);
        }
        else
        {
            Text("����").Write(40, 205, Color::RED);
        }

        Font::Set(TypeFont::GOSTAU16BOLD);
    }

    static Page page(items, FuncDraw, nullptr);

    Page *self = &page;


    void UpdateInput()
    {
        while (!Keyboard::Empty())
        {
            Control control = Keyboard::NextControl();

            if (!control.IsPress())
            {
                continue;
            }

            if (control.key == Key::OK)
            {
                switch (state)
                {
                case State::Normal:
                    break;
                }
            }
            else
            {
                if (state == State::Normal)
                {
                    Menu::Input::OnControl(control);
                }
            }
        }
    }
}


int PageCalibration::GetDisplayVoltage()
{
    if (type_signal == 0)           // 1
    {
        int values[2][4] =
        {
            {-150, INVALID_VOLTAGE, INVALID_VOLTAGE, INVALID_VOLTAGE},
            {-600, INVALID_VOLTAGE, INVALID_VOLTAGE, INVALID_VOLTAGE}
        };

        return values[type_accum][current_point];
    }

    return INVALID_VOLTAGE;
}


void PageCalibration::EnableOutput()
{
    output_en = true;
}


void PageCalibration::DisableOutput()
{
    output_en = false;
}


void PageCalibration::SaveCalibrationFactor()
{
    state = State::FactorSave;

    Timer::SetDefferedOnceTask(TimerTask::Calibrate, TIME_TIMER, TimerFunction);
}


void PageCalibration::ChangeCalibrationFactor(int dir)
{
    SettingsCal::StructCal &cal = gset.cal.cal[type_signal][type_accum][current_point];

    if (dir > 0)
    {
        cal.k *= 1.05f;
    }
    else
    {
        cal.k /= 1.05f;
    }
}


void PageCalibration::TimerFunction()
{
    state = State::Normal;
}
