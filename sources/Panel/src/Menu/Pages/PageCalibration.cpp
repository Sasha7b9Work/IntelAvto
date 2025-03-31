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

    static uint8 type_signal = 0;
    static uint8 type_accum = 0;
    static uint8 num_point = 0;        // 0 - максимальный размах
    static bool output_en = false;

    static int GetDisplayVoltage();

    static void EnableOutput();

    static void DisableOutput();

    // Сохранить калибровочный коэффициент
    static void SaveCalibrationFactor();

    // dir = 1/-1 Изменяет калибровочный коэффициент в бОльшую (1) или меньшую (-1) стОроны
    static void ChangeCalibrationFactor(int dir);

    static void TimerFunction();

    struct State
    {
        enum E
        {
            Normal,
            FactorSave,         // Калибровочный коэффициент сохранён
            FactroNotSave       // Калибровочный коэффициент не сохранён
        };
    };

    static State::E state = State::Normal;

    static void FuncPress_Back()
    {
        Menu::SetOpenedPage(PageMain::self);
    }

    DEF_BUTTON(bBack,
        "Назад",
        FuncPress_Back
    );

    DEF_CHOICE_1_FULL(chTypeSignal,
        "Импульс",
//        "1", "2a", "3a", "3b",
        "1",
        type_signal,
        FuncVV
    );

    DEF_CHOICE_1_FULL(chTypeAccum,
        "Аккумулятор",
//        "12В", "24В",
        "12В",
        type_accum,
        FuncVV
    );

    DEF_CHOICE_1_FULL(chTypePoint,
        "Точка",
//        "1", "2", "3", "4",
        "1",
        num_point,
        FuncVV
    );

    static void FuncPress_Reset()
    {
        
    }

    DEF_BUTTON(bReset,
        "Сброс",
        FuncPress_Reset
    );

    static Item *items[] =
    {
        &bBack,
        &chTypeSignal,
        &chTypeAccum,
        &chTypePoint,
//        &bReset,
        nullptr
    };

    static void FuncDraw()
    {
        int dy = 25;

        if (state == State::Normal)
        {
            int x = 180;
            int y = 20;

            Text("1. Нажмите кнопку START.").Write(x, y, Color::WHITE);                 y += dy;

            Text("2. Вращением ручки установите").Write(x, y);                          y += dy;

            Text("   амлитуду выходного сигнала").Write(x, y);                          y += dy;

            Text("   %d В (контроль по осцилло-", GetDisplayVoltage()).Write(x, y);     y += dy;

            Text("   графу).").Write(x, y);                                             y += dy;

            Text("3. Нажмите кнопку ОК, чтобы").Write(x, y);                            y += dy;

            Text("   сохранить калибровочный").Write(x, y);                             y += dy;

            Text("   коэффиициент, либо кнопку ESC,").Write(x, y);                      y += dy;

            Text("   чтобы не сохранять.").Write(x, y);                                 y += dy;
        }
        else if (state == State::FactorSave)
        {
            int x = 180;
            int y = 70;

            Text("Коэффициент").Write(x, y, Display::Width() - x, Color::GREEN); y += 30;

            Text("cохранен").Write(x, y, Display::Width() - x);
        }
        else if (state == State::FactroNotSave)
        {
            int x = 180;
            int y = 70;

            Text("Коэффициент").Write(x, y, Display::Width() - x, Color::RED); y += 30;

            Text("не cохранен").Write(x, y, Display::Width() - x);
        }

        Font::Set(TypeFont::GOSTB28B);

        Text("Выход:").Write(30, 150);

        if (output_en)
        {
            Text("ВКЛ").Write(45, 205, Color::GREEN);
        }
        else
        {
            Text("ОТКЛ").Write(40, 205, Color::RED);
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

            if (output_en)
            {
                if (control.key == Key::Stop || control.key == Key::Esc)
                {
                    DisableOutput();

                    state = State::FactroNotSave;

                    Timer::SetDefferedOnceTask(TimerTask::Calibrate, 1000, TimerFunction);
                }
                else if (control.key == Key::OK)
                {
                    DisableOutput();

                    SaveCalibrationFactor();
                }
                else if (control.key == Key::GovLeft)
                {
                    DisableOutput();

                    ChangeCalibrationFactor(-1);

                    EnableOutput();
                }
                else if (control.key == Key::GovRight)
                {
                    DisableOutput();

                    ChangeCalibrationFactor(1);

                    EnableOutput();
                }
            }
            else
            {
                if (control.key == Key::Start)
                {
                    EnableOutput();
                }
                else
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

        return values[type_accum][num_point];
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

    Timer::SetDefferedOnceTask(TimerTask::Calibrate, 1000, TimerFunction);
}


void PageCalibration::ChangeCalibrationFactor(int dir)
{
    SettingsCal::StructCal &cal = gset.cal.cal[type_signal][type_accum][num_point];

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
