// 2023/12/22 15:42:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"
#include "Display/Display_.h"
#include "FlashDrive/FlashDrive.h"


namespace PageMain
{
    static void FuncPress_Tests()
    {
        Menu::SetOpenedPage(PageSignal1::self);
    }

    DEF_BUTTON(bTests,
        "Тесты",
        FuncPress_Tests
    );

    static void FuncPress_Settings()
    {
//        Menu::SetOpenedPage(PageSettings::self);
    }

    DEF_BUTTON(bSettings,
        "Настройки",
        FuncPress_Settings
    );

    static void FuncPress_Calibration()
    {
        Menu::SetOpenedPage(PageCalibration::self);
    }

    DEF_BUTTON(bCalibration,
        "Калибровка",
        FuncPress_Calibration
    );

    static void FuncPress_FlashDrive()
    {

    }

    DEF_BUTTON(bFlashDrive,
        "Флешка",
        FuncPress_FlashDrive
    );

    static void FuncPress_Info()
    {
        Menu::SetOpenedPage(PageInfo::self);
    }

    DEF_BUTTON(bInfo,
        "Информация",
        FuncPress_Info
    );

    static void FuncPress_Display()
    {
        Display::Init();
    }


    DEF_BUTTON(bDisplay,
        "Дисплей",
        FuncPress_Display
    );

    static Item *items[] =
    {
        &bTests,
        &bSettings,
        &bFlashDrive,
        &bCalibration,
        &bInfo,
        &bDisplay,
        nullptr
    };

    static void OnDraw_Page()
    {
        bFlashDrive.SetActive(FDrive::IsConnected());
    }

    static Page page(items, OnDraw_Page, nullptr);
}


Page *PageMain::self = &PageMain::page;
