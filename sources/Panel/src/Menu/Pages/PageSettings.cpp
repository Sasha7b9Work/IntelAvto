// 2025/02/03 10:37:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"


namespace PageSettings
{
    static void FuncPress_Back()
    {
        Menu::SetOpenedPage(PageMain::self);
    }

    DEF_BUTTON(bBack,
        "Назад",
        FuncPress_Back
    );

    static void FuncPress_LAN()
    {

    }

    DEF_BUTTON(bLAN,
        "LAN",
        FuncPress_LAN
    );

    static Item *items[] =
    {
        &bBack,
        &bLAN,
        nullptr
    };

    static Page page(items, nullptr, nullptr);

    Page *self = &page;
}
