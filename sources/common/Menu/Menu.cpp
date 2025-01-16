// 2023/09/08 20:52:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Display/Font/Font.h"
#include "Hardware/Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Pages.h"
#include "Device/Device.h"
#include <cstring>


namespace Menu
{
    void SubscribeToEvents();

    // Текущая отображаемая страница меню
    Page *openedPage = PageSignal1::self;

    void (*funcUpdate)() = nullptr;

    namespace Input
    {
        // Обработка события кнопки
        void OnControl(const Control &);

        // Обработка события ручки
        void OnGovernorRotate(const Control &);

        // Обработка нажатия Enter и GovButton
        bool OnGovernorButton(const Control &);
    }
}


void Menu::Draw()
{
    openedPage->DrawMenuItem(0, 0, 0);
}


void Menu::Input::OnGovernorRotate(const Control &)
{
}


Page *Menu::OpenedPage()
{
    return openedPage;
}


void Menu::SetOpenedPage(Page *page)
{
    openedPage = page;
}


void Menu::Input::OnControl(const Control &control)
{
    if (!openedPage->OnEventControl(control))
    {
        if (!openedPage->SelectedItem()->OnEventControl(control))
        {
            if (control.IsRelease() && control.key == Key::Start)
            {
                Device::IsRunning() ? Device::Stop() : Device::Run();
            }
        }
    }
}


bool Menu::Input::OnGovernorButton(const Control &)
{
    return false;
}


void Menu::Init()
{
    Input::SetFuncUpdate(Input::FuncUpdate);

    SubscribeToEvents();
}


void Menu::Input::Update()
{
    funcUpdate();
}


void Menu::Input::FuncUpdate()
{
    while (!Keyboard::Empty())
    {
        Control control = Keyboard::NextControl();

        OnGovernorRotate(control);

        OnControl(control);
    }
}


void Menu::Input::FuncEmptyUpdate()
{
    while (!Keyboard::Empty())
    {
        Keyboard::NextControl();
    }
}


void Menu::Input::SetFuncUpdate(void (*_funcUpdate)())
{
    funcUpdate = _funcUpdate;
}


void Menu::SubscribeToEvents()
{
}
