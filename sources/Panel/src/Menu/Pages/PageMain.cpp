// 2023/12/22 15:42:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"


namespace PageMain
{
    static void FuncPress_Tests()
    {
        Menu::SetOpenedPage(PageSignal1::self);
    }

    DEF_BUTTON(bTests,
        "�����",
        FuncPress_Tests
    );

    static void FuncPress_Settings()
    {
        Menu::SetOpenedPage(PageSettings::self);
    }

    DEF_BUTTON(bSettings,
        "���������",
        FuncPress_Settings
    );

    static void FuncPress_Info()
    {

    }

    DEF_BUTTON(bInfo,
        "����������",
        FuncPress_Info
    );

    static Item *items[] =
    {
        &bTests,
        &bSettings,
        &bInfo,
        nullptr
    };

    static Page page(items, nullptr, nullptr);
}


Page *PageMain::self = &PageMain::page;
