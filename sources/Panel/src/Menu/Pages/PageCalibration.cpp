// 2025/03/25 09:30:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Display/Text_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "Display/Display_.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/InputField.h"


using namespace Primitives;


namespace PageCalibration
{
    static const int INVALID_VOLTAGE = 999999999;
    static const uint TIME_TIMER = 1500;

    static const int NUM_SIGNALS = 4;
    static uint8 type_signal = 0;
    static const int NUM_ACCUM = 2;
    static uint8 type_accum = 0;
    static const int NUM_POINTS = 2;        // ������� ����� ����� ��� ����������
    static int  current_point = 0;          // �����, ������� ������ ��������
    static bool output_en = false;

    // ����� ���������� ��������, �������� �������
    static int values[NUM_SIGNALS][NUM_ACCUM][NUM_POINTS] =
    {
        {                   // 1
            { 0, 0 },
            { 0, 0 }
        },
        {                   // 2a
            { 0, 0 },
            { 0, 0 }
        },
        {                   // 3a
            { 0, 0 },
            { 0, 0 }
        },
        {                   // 3b
            { 0, 0 },
            { 0, 0 }
        }
    };

    static InputField field;

    // ������ ���������� �� ����� � ������������ � �������������� �����������
    static void EnableOutput();

    // ����� ���������� � ������
    static void DisableOutput();

    // ���������� ���������� ������� �����
    static int GetVoltagePoint();

    // ��������� ����������� ������������� ������������ ��� ������������ ��������������
    static void StoreCalibrateFactors();

    // ������������ ����� ���������� ������������
    static void RestoreCalibrateFactors();

    // ���������� � ���������� ������������� �������
    static void CalculateCalibrateFactors();

    static void TimerFunction();

    struct State
    {
        enum E
        {
            Normal,                     // ��������� ���������
            EnableOutputAndWaitEnter,   // ����������� ���������� �� ������ � ��������� ���� ����������� ��������
            ConfirmForSave,             // ������� ������������� �� ���������� ��������
            FactorSave,                 // ������������� ����������� �������
            FactroNotSave               // ������������� ����������� �� �������
        };
    };

    static bool FieldIsVisible();

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

    static void FuncPress_Calibrate()
    {
        if (state == State::Normal)
        {
            state = State::EnableOutputAndWaitEnter;

            StoreCalibrateFactors();

            current_point = 0;

            EnableOutput();
        }
        else if (state == State::EnableOutputAndWaitEnter)
        {
            if (field.GetValueMilliUnits() != 0)
            {
                values[type_signal][type_accum][current_point] = field.GetValueMilliUnits();

                if (current_point < NUM_POINTS - 1)
                {
                    current_point++;

                    EnableOutput();
                }
                else
                {
                    DisableOutput();

                    state = State::ConfirmForSave;
                }

                field.Reset();
            }
        }
        else if (state == State::ConfirmForSave)
        {

        }
        else if (state == State::FactorSave)
        {

        }
        else if (state == State::FactroNotSave)
        {

        }
    }

    DEF_BUTTON(bCalibrate,
        "�����������",
        FuncPress_Calibrate
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
        &bCalibrate,
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
        else if (state == State::EnableOutputAndWaitEnter)
        {
            Text("����� %d", current_point + 1).Write(250, 45, Color::WHITE);

            field.Draw(240, 150, FieldIsVisible());
        }
        else if (state == State::ConfirmForSave)
        {
            for (int i = 0; i < NUM_POINTS; i++)
            {
                int y = 20 + 25 * i;

                Text("����� %d", i + 1).Write(200, y, Color::WHITE);
                Text("%d �", values[type_signal][type_accum][i]).Write(300, y);
            }

            int x = 170;
            int y = 125;

            Text("��� ���������� �������������").Write(x, y);
            Text("������������� ������� OK.").Write(x, y + 25);

            Text("��� ������ ������� Esc.").Write(x, y + 75);
        }
        else if (state == State::FactorSave)
        {
            int x = 190;
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

        Text("�����:").Write(30, 150, Color::WHITE);

        if (output_en)
        {
            Text("%d �", GetVoltagePoint()).Write(45, 205, Color::WHITE);
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
            const Key::E key = Keyboard::NextKey();

            if (key == Key::Start || key == Key::Stop)
            {
            }
            else if (FieldIsVisible() && ((key >= Key::_0 && key <= Key::_9) || key == Key::Dot || key == Key::Esc))
            {
                field.OnKey(key);
            }
            else if (state == State::ConfirmForSave)
            {
                if (key == Key::OK)
                {
                    CalculateCalibrateFactors();
                }
                else if (key == Key::Esc)
                {
                    RestoreCalibrateFactors();
                }
            }
            else if (state == State::FactorSave || state == State::FactroNotSave)
            {
            }
            else
            {
                if (state == State::Normal || key == Key::OK)
                {
                    Menu::Input::OnKey(key);
                }
            }
        }
    }
}


void PageCalibration::EnableOutput()
{
    output_en = true;
}


void PageCalibration::DisableOutput()
{
    output_en = false;
}


void PageCalibration::CalculateCalibrateFactors()
{
    state = State::FactorSave;

    Timer::SetDefferedOnceTask(TimerTask::Calibrate, TIME_TIMER, TimerFunction);
}


void PageCalibration::TimerFunction()
{
    state = State::Normal;
}


int PageCalibration::GetVoltagePoint()
{
    static const int voltages[NUM_SIGNALS][NUM_ACCUM][NUM_POINTS] =
    {
        {                                                                           // 1
            { -75,  -150 },
            { -300, -600 }
        },
        {                                                                           // 2a
            { 37, 112 },
            { 37, 112 }
        },
        {                                                                           // 3a
            { INVALID_VOLTAGE, INVALID_VOLTAGE },
            { INVALID_VOLTAGE, INVALID_VOLTAGE }
        },
        {                                                                           // 3b
            { 75,  100 },
            { 150, 200 }
        }
    };

    return voltages[type_signal][type_accum][current_point];
}


bool PageCalibration::FieldIsVisible()
{
    return state == State::EnableOutputAndWaitEnter;
}


void PageCalibration::StoreCalibrateFactors()
{

}


void PageCalibration::RestoreCalibrateFactors()
{
    state = State::FactroNotSave;

    Timer::SetDefferedOnceTask(TimerTask::Calibrate, 3000, TimerFunction);
}
