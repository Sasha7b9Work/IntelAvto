// 2025/02/21 08:42:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"


namespace PageLAN
{
    static void FuncPress_Back()
    {
        Menu::SetOpenedPage(PageSettings::self);
    }

    DEF_BUTTON(bBack,
        "Назад",
        FuncPress_Back
    );

    static void FuncPress_IT6523()
    {
        Menu::SetOpenedPage(PageIT6523::self);
    }

    DEF_BUTTON(bIT6523,
        "IT6523",
        FuncPress_IT6523
    );

    static Item *items[] =
    {
        &bBack,
        &bIT6523,
        nullptr
    };

    static Page page(items, nullptr, nullptr);

    Page *self = &page;


    namespace PageIT6523
    {
        static void FuncPress_Back()
        {
            Menu::SetOpenedPage(PageLAN::self);
        }

        DEF_BUTTON(bBack,
            "Назад",
            FuncPress_Back
        );

        static Item *items[] =
        {
            &bBack,
            nullptr
        };

        static Page page(items, nullptr, nullptr);

        Page *self = &page;
    }

}
