// 2025/03/25 09:30:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"


namespace PageCalibration
{
    static uint8 type_signal = 0;
    static uint8 type_accum = 0;
    static uint8 type_point = 0;

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

    DEF_CHOICE_4_FULL(chTypeSignal,
        "Импульс",
        "1", "2a", "3a", "3b",
        type_signal,
        FuncVV
    );

    DEF_CHOICE_2_FULL(chTypeAccum,
        "Аккумулятор",
        "12В", "24В",
        type_accum,
        FuncVV
    );

    DEF_CHOICE_4_FULL(chTypePoint,
        "Точка",
        "1", "2", "3", "4",
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
        &bReset,
        nullptr
    };

    static void FuncDraw()
    {

    }

    static Page page(items, FuncDraw, nullptr);

    Page *self = &page;
}
