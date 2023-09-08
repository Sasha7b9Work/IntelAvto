// 2022/01/13 13:50:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Menu/Menu.h"
#include "Settings.h"
#include "Menu/MenuItemsDef.h"


static void OnPress_Exit()
{
    Menu::SetOpenedPage(PageColors::self);
    glob_set.Save();
}


DEF_BUTTON(bExit,
    "Выход", "Exit",
    "Переход на страницу СЕРВИС", "Go to the SERVICE page",
    OnPress_Exit
);


extern Choice cColor;
extern GovernorChannelColor gRed;
extern GovernorChannelColor gGreen;
extern GovernorChannelColor gBlue;

static Color colors[4] =
{
    Color::FILL,
    Color::BACK,
    Color::MENU_UNSELECT,
    Color::MENU_SELECT
};


static void OnChanged_Color()
{
    Color color = colors[cColor.Value()];

    gRed.SetValue(color.GetRED());
    gGreen.SetValue(color.GetGREEN());
    gBlue.SetValue(color.GetBLUE());

    cColor.SetColorBackground(colors[cColor.Value()]);
}


static void OnChanged_Red(uint8)
{
    colors[cColor.Value()].SetRED((uint8)gRed.Value());
}


static void OnChanged_Green(uint8)
{
    colors[cColor.Value()].SetGREEN((uint8)gGreen.Value());
}


static void OnChanged_Blue(uint8)
{
    colors[cColor.Value()].SetBLUE((uint8)gBlue.Value());
}


DEF_CHOICE_4(cColor,
    "Выбор настраиваемого элемента", "Selecting a custom item",
    "Текст", "Фон",        "Меню 1", "Меню 2",
    "Text",  "Background", "Menu 1", "Menu 2",
    OnChanged_Color
);


DEF_COLOR_GOVERNOR(gRed,
    GovernorChannelColor::Red,
    "Настройка красного", "Red setting",
    OnChanged_Red
);


DEF_COLOR_GOVERNOR(gGreen,
    GovernorChannelColor::Green,
    "Настройка зеленого", "Green setting",
    OnChanged_Green
);


DEF_COLOR_GOVERNOR(gBlue,
    GovernorChannelColor::Blue,
    "Настройка синего", "Blue setting",
    OnChanged_Blue
);


static Item *items[7] =
{
    &bExit,
    &cColor,
    &gRed,
    &gGreen,
    &gBlue,
    nullptr
};


static Page page(items, nullptr, nullptr, true);


Page *PageTuneColors::self = &page;


void PageTuneColors::Init()
{
    OnChanged_Color();
}
