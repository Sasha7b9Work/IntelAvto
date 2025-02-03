// 2023/12/22 15:42:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Pages.h"


namespace PageMain
{
    static void FuncPress_Settings()
    {

    }

    DEF_BUTTON(bSettings,
        "Настройки",
        FuncPress_Settings
    );

    static Item *items[] =
    {
        &bSettings,
        nullptr
    };

    static Page page(items, nullptr, nullptr, nullptr);
}


Page *PageMain::self = &PageMain::page;
