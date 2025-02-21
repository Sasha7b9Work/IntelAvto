// 2025/02/21 08:42:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"


namespace PageLAN
{
    DEF_BUTTON(bBack,
        "Назад",
        []()
        {
            Menu::SetOpenedPage(PageSettings::self);
        }
    );

    DEF_BUTTON(bIT6523,
        "IT6523",
        []()
        {
            Menu::SetOpenedPage(PageIT6523::self);
        }
    );

    DEF_BUTTON(bSCPI,
        "SCPI",
        []()
        {
            Menu::SetOpenedPage(PageSCPI::self);
        }
    );

    DEF_BUTTON(bNetmask,
        "Маска сети",
        []()
        {

        }
    );

    DEF_BUTTON(bGateway,
        "Gateway",
        []()
        {

        }
    );

    DEF_BUTTON(bMAC,
        "MAC",
        []()
        {

        }
    );

    static Item *items[] =
    {
        &bBack,
        &bIT6523,
        &bSCPI,
        &bNetmask,
        &bGateway,
        &bMAC,
        nullptr
    };

    static Page page(items, nullptr, nullptr);

    Page *self = &page;


    namespace PageIT6523
    {
        DEF_BUTTON(bBack,
            "Назад",
            []()
            {
                Menu::SetOpenedPage(PageLAN::self);
            }
        );

        IAddressIP bIP{
            "Адрес IP",
            gset.ipIT6523
        };

        DEF_BUTTON(bPort,
            "Порт",
            []()
            {

            }
        );

        static Item *items[] =
        {
            &bBack,
            &bIP,
            &bPort,
            nullptr
        };

        static Page page(items, nullptr, nullptr);

        Page *self = &page;
    }


    namespace PageSCPI
    {
        DEF_BUTTON(bBack,
            "Назад",
            []()
            {
                Menu::SetOpenedPage(PageLAN::self);
            }
        );

        IAddressIP bIP{
            "Адрес IP",
            gset.ipSCPI
        };

        DEF_BUTTON(bPort,
            "Порт",
            []()
            {
    
            }
        );

        static Item *items[] =
        {
            &bBack,
            &bIP,
            &bPort,
            nullptr
        };

        static Page page(items, nullptr, nullptr);

        Page *self = &page;
    }
}
