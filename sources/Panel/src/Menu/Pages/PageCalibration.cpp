// 2025/03/25 09:30:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"


namespace PageCalibration
{
    static uint8 type_signal = 0;
    static uint8 type_accum = 0;

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

    static Item *items[] =
    {
        &bBack,
        &chTypeSignal,
        &chTypeAccum,
        nullptr
    };

    static Page page(items, nullptr, nullptr);

    Page *self = &page;
}
