// 2023/09/08 20:52:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Font/Font.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Pages.h"
#include "Device/Device.h"
#include "Hardware/Timer.h"
#include <cstring>


namespace Menu
{
    // Текущая отображаемая страница меню
    Page *openedPage = PageSignal1::self;

    void (*funcUpdate)() = nullptr;

    Primitives::Label labelMode(100, 5, "СТОП", Color::BLACK, Color::GREEN, Color::WHITE);
}


void Menu::Draw()
{
    openedPage->Draw(0, 0, 0);

    if (Page::IsSignal(OpenedPage()))
    {
        labelMode.Draw();
    }
}


Page *Menu::OpenedPage()
{
    return openedPage;
}


void Menu::SetOpenedPage(Page *page)
{
    openedPage = page;

    if (openedPage == PageCalibration::self)
    {
        Menu::Input::SetFuncUpdate(PageCalibration::UpdateInput);
    }
    else
    {
        Menu::Input::SetFuncUpdate(Menu::Input::FuncUpdate);
    }
}


void Menu::Init()
{
    Input::SetFuncUpdate(Input::FuncUpdate);

    labelMode.SetColors(Color::BLACK, Color::GREEN, Color::WHITE);
}


void Menu::Input::Update()
{
    funcUpdate();
}


void Menu::Input::FuncUpdate()
{
}


void Menu::Input::FuncEmptyUpdate()
{
}


void Menu::Input::SetFuncUpdate(void (*_funcUpdate)())
{
    funcUpdate = _funcUpdate;
}


bool Menu::OpenedPageIsSignal()
{
    Page *page = OpenedPage();

    return
        page == PageSignal1::self ||
        page == PageSignal2a::self ||
        page == PageSignal2b::self ||
        page == PageSignal3a::self ||
        page == PageSignal3b::self ||
        page == PageSignal4::self ||
        page == PageSignal5a::self ||
        page == PageSignal5b::self;
}
