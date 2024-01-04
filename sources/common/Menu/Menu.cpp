// 2023/09/08 20:52:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Display/Font/Font.h"
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Pages.h"
#include <cstring>


namespace Menu
{
    void SubscribeToEvents();

    // ������� ������������ �������� ����
    Page *openedPage = PageSignal1::self;

    void (*funcUpdate)() = nullptr;

    namespace Input
    {
        // ��������� ������� ������
        bool OnControl(const Control &);

        // ��������� ������� �����
        void OnGovernorRotate(const Control &);

        // ��������� ������� Enter � GovButton
        bool OnGovernorButton(const Control &);
    }
}


void Menu::Draw()
{
    openedPage->Draw(0, 0, 0);
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


bool Menu::Input::OnControl(const Control &control)
{
    if (OnGovernorButton(control))
    {
        return true;
    }

    switch (control.key)
    {
    case Key::GovButton:
        if (openedPage->SelectedItem())
        {
            openedPage->SelectedItem()->OnEnterKeyGovernor(control);
        }
        break;

    case Key::Right:
        openedPage->OnKeyRight();
        break;

    case Key::Left:
        openedPage->OnKeyLeft();
        break;

    case Key::Count:
        break;
    case Key::GovLeft:
    case Key::GovRight:
        Menu::OpenedPage()->SelectedItem()->OnRotateGovernor(control);
        break;
    case Key::None:
    case Key::_1:
    case Key::_2:
    case Key::_3:
    case Key::_4:
    case Key::_5:
    case Key::_6:
    case Key::_7:
    case Key::_8:
    case Key::_9:
    case Key::_0:
    case Key::Minus:
    case Key::Dot:
    case Key::Start:
    case Key::Back:
    case Key::OK:
        break;
    }

    return false;
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
