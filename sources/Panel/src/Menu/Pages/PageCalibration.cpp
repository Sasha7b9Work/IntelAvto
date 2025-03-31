// 2025/03/25 09:30:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Display/Text_.h"


using namespace Primitives;


namespace PageCalibration
{
    static const int INVALID_VOLTAGE = 999999999;

    static uint8 type_signal = 0;
    static uint8 type_accum = 0;
    static uint8 type_point = 0;        // 0 - максимальный размах

    static int GetDisplayVoltage()
    {
        if (type_signal == 0)           // 1
        {
            int values[2][4] =
            {
                {-150, INVALID_VOLTAGE, INVALID_VOLTAGE, INVALID_VOLTAGE},
                {-600, INVALID_VOLTAGE, INVALID_VOLTAGE, INVALID_VOLTAGE}
            };

            return values[type_accum][type_point];
        }

        return INVALID_VOLTAGE;
    }

    struct State
    {
        enum E
        {
            None,
            DrawWarningMessageReset         // Выводим предупреждающее сообщение про сброс калибровочных коэффициентов
        };
    };

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
        type_point,
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
        int x = 180;
        int y = 10;
        int dy = 25;

        Text("1. Нажмите кнопку START.").Write(x, y, Color::WHITE);                 y += dy;

        Text("2. Вращением ручки установите").Write(x, y);                          y += dy;

        Text("   амлитуду выходного сигнала").Write(x, y);                          y += dy;

        Text("   %d В (контроль по осцилло-", GetDisplayVoltage()).Write(x, y);     y += dy;

        Text("   графу).").Write(x, y);                                             y += dy;

        Text("3. Нажмите кнопку ОК, чтобы").Write(x, y);                            y += dy;

        Text("   сохранить калибровочный").Write(x, y);                             y += dy;

        Text("   коэффиициент, либо кнопку ESC,").Write(x, y);                      y += dy;

        Text("   чтобы отменить.").Write(x, y);                                     y += dy;
    }

    static Page page(items, FuncDraw, nullptr);

    Page *self = &page;
}
