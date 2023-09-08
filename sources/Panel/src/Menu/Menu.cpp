// 2023/09/08 20:52:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings.h"
#include "Display/Display.h"
#include "Display/Font/Font.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageIndication.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstring>


namespace Menu
{
    void SubscribeToEvents();

    // Текущая отображаемая страница меню
    Page *openedPage = PageIndication::self;

    void (*funcUpdate)() = nullptr;

    namespace Input
    {
        // Обработка события кнопки
        bool OnControl(const Control &);

        // Открывает страницу, соответствующую воздействию control. Возвращает false, если для воздействия нет соответствующей страницы
        bool OpenPage(const Control &);

        // Обработка события ручки
        void OnGovernorRotate(const Control &);

        // Обработка нажатия Enter и GovButton
        bool OnGovernorButton(const Control &);
    }
}


void Menu::Draw()
{
    openedPage->Draw(0, Display::PHYSICAL_HEIGHT - Item::HEIGHT - 1, 0);
}


void Menu::Input::OnGovernorRotate(const Control &)
{
}


bool Menu::Input::OpenPage(const Control &control)
{
    if (!control.action.IsPress())
    {
        return false;
    }
           
    Page *const pages[Control::Count] =
    {
        nullptr,              // GovButton,
        nullptr,              // Mode,
        PageIndication::self, // Indication,
        nullptr,              // Left,
        nullptr,              // Right,
        nullptr,              // Channels,
        nullptr,              // Enter,
        nullptr,              // Service,
        nullptr,              // GovLeft,
        nullptr,              // GovRight,
        nullptr,              // Test,
        nullptr,              // Auto,
        nullptr               // None,
    };

    Page *page = pages[control.value];

    if (page && page != openedPage)
    {
        openedPage = page;
        Hint::Hide();
        return true;
    }

    return false;
}


Page* Menu::OpenedPage()
{
    return openedPage;
}


void Menu::SetOpenedPage(Page *page)
{
    openedPage = page;
}


bool Menu::Input::OnControl(const Control &control)
{
    if (PageIndication::calibrationMode.IsEnabled() &&
        control.value != Control::GovButton &&
        control.value != Control::GovLeft &&
        control.value != Control::GovRight)
    {
        PageIndication::calibrationMode.value = CalibrationMode::Disabled;
        
        return true;
    }

    if (OnGovernorButton(control))
    {
        return true;
    }

    switch (control.value)
    {
    case Control::GovButton:
    case Control::Enter:
        if (openedPage->SelectedItem())
        {
            openedPage->SelectedItem()->OnEnterKeyGovernor(control);
        }
        break;

    case Control::Right:
        openedPage->OnKeyRight();
        break;

    case Control::Left:
        openedPage->OnKeyLeft();
        break;

    case Control::Mode:
        Hint::Hide();
        return true;

    case Control::Channels:
        return true;

    case Control::Test:
        break;

    case Control::Auto:
        break;

    case Control::Indication:
    case Control::Service:
    case Control::Count:
        break;
    case Control::GovLeft:
    case Control::GovRight:
        Menu::OpenedPage()->SelectedItem()->OnRotateGovernor(control);
        break;
    case Control::None:
    default:
        // никаких действий по умолчанию производить не требуется
        break;
    }

    return false;
}


bool Menu::Input::OnGovernorButton(const Control &control)
{
    if (control.value == Control::GovButton)
    {
        if (PageIndication::launchSource == LaunchSource::OneTime)
        {
            PageIndication::OnceLaunchSwitchTrue();
            return true;
        }
    }

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

        if (!OnControl(control))
        {
            OpenPage(control);
        }

        Display::Refresh();
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
