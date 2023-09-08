// 2022/01/12 11:54:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Display/Primitives.h"
#include "Settings.h"
#include "Menu/Pages/Pages.h"


static void OnPress_Exit()
{
    Menu::SetOpenedPage(PageService::self);
    glob_set.Save();
}


DEF_BUTTON(bExit,
    "�����", "Exit",
    "������� �� �������� ������", "Go to the SERVICE page",
    OnPress_Exit
);


static void OnPress_Tune()
{
    Menu::SetOpenedPage(PageTuneColors::self);
}


DEF_BUTTON(bTune,
    "���������",        "Tune",
    "��������� ������", "Tune colors",
    OnPress_Tune
);


static void OnPress_Reset()
{
    glob_set.Reset();
    glob_set.Save();
    PageTuneColors::Init();
}


DEF_BUTTON(bReset,
    "�����", "Reset",
    "�������� ����� �� �������� �� ���������", "Reset colors to default",
    OnPress_Reset
);


static void OnChanged_Scheme()
{
    PageTuneColors::Init();
}


DEF_CHOICE_3(cScheme,
    "����� �������� �����", "Choosing a color scheme",
    "����� 1", "����� 2", "����� 3",
    "Scheme 1", "Scheme 2", "Scheme 3",
    glob_set.colorScheme, OnChanged_Scheme
);


static Item *items[7] =
{
    &bExit,
    &cScheme,
    &bTune,
    &bReset,
    nullptr
};


static Page page(items, nullptr, nullptr, true);


Page *PageColors::self = &page;
