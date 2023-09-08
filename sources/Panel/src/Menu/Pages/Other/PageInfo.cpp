// 2022/01/11 09:34:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Display/Display.h"
#include "Settings.h"
#include "Menu/Pages/Pages.h"


using namespace Primitives;


//static void Draw_Info1()
//{
//    Rectangle(470, 225).FillRounded(5, 5, 1, Color::BACK, Color::FILL);
//
//    Text(NAME_MODEL).Write(0, 35, Display::PHYSICAL_WIDTH, Color::FILL);
//
//    Text("---------------------------------------------").Write(40, 68);
//
//    int x0 = 250;
//    int y = 100;
//    int dy = 30;
//    int x = 100;
//
//    Text(LANG_IS_RU ? "Производитель" : "Manufacturer").Write(x, y);
//    Text(LANG_IS_RU ? ": ОАО МНИПИ" : " : OAO MNIPI").Write(x0, y);
//
//    y += dy;
//
//    Text(LANG_IS_RU ? "Программное обеспечение" : "Software").Write(x, y);
//
//    y += dy;
//
//    x += 50;
//
//    Text(LANG_IS_RU ? "Версия" : "Version").Write(x, y);
//    Text(String("%: %s", STR_NUM_VERSION)).Write(x0, y);
//
//    y += dy;
//
//    Text("CRC32").Write(x, y);
//    Text(String(": %s", STR_CRC32)).Write(x0, y);
//}


static void Draw_Info()
{
    Rectangle(470, 225).FillRounded(5, 5, 1, Color::BACK, Color::FILL);

    Text(LANG_IS_RU ? "Электронно-счетные частотомеры ОАО МНИПИ" :
                      "Electronic calculating frequency meters OAO MNIPI").Write(0, 35, Display::PHYSICAL_WIDTH, Color::FILL);

    Text("---------------------------------------------").Write(40, 68);

    int y = 110;
    int dy = 30;
    int x = 75;

    Text("Ч3-96         0.01 Гц - 200 МГц, 3.2 ГГц").Write(x, y);

    y += dy;

    Text("Ч3-96/1      0.01 Гц - 200 МГц, 10 ГГц").Write(x, y);

    y += dy;

    Text("Ч3-96/2      0.01 Гц - 250 МГц, 18 ГГц").Write(x, y);
}


static void OnPress_Exit()
{
    Menu::SetOpenedPage(PageService::self);
}


DEF_BUTTON(bExit,
    "Выход", "Exit",
    "Переход на страницу СЕРВИС", "Go to the SERVICE page",
    OnPress_Exit
);


static Item* items[7] =
{
    &bExit,
    nullptr
};


static Page pageInfo(items, nullptr, Draw_Info);


Page* PageInfo::self = &pageInfo;
