// 2025/02/03 10:37:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Pages.h"


namespace PageSettings
{
    static void FuncPress_LAN()
    {

    }

    DEF_BUTTON(bLAN,
        "LAN",
        FuncPress_LAN
    );

    static Item *items[] =
    {
        &bLAN,
        nullptr
    };

    static Page page(items, nullptr, nullptr, nullptr);
}

Page *PageSettings::self = &PageSettings::page;
