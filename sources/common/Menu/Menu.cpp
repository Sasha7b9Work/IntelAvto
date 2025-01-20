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
#include "Hardware/Timer.h"
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
        void OnControl(const Control &);

        // ��������� ������� �����
        void OnGovernorRotate(const Control &);

        // ��������� ������� Enter � GovButton
        bool OnGovernorButton(const Control &);
    }

    Primitives::Label labelMode(10, 170, "����", Color::BLACK, Color::GREEN, Color::WHITE);
}


void Menu::Draw()
{
    openedPage->DrawMenuItem(0, 0, 0);

    if (Page::IsSignal(OpenedPage()))
    {
        labelMode.Draw();
    }
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
    if (!Device::IsStopped())                                               // ����� ��� ����,
    {
        if (control.key != Key::Start && control.key != Key::Stop)          // �� ������������ ������ ����� � ����
        {
            return;
        }
    }

    if (!openedPage->OnEventControl(control))
    {
        if (!openedPage->SelectedItem()->OnEventControl(control))
        {
            if (control.IsRelease())
            {
                if (control.key == Key::Start)
                {
                    if (Device::IsStopped() || Device::InPause())
                    {
                        labelMode.SetState("����", Color::WHITE, Color::RED);

                        Device::Start();
                    }
                    else if (Device::IsRunning())
                    {
                        labelMode.SetState("�����", Color::BLACK, Color::YELLOW);

                        Device::Pause();
                    }
                }
                else if (control.key == Key::Stop)
                {
                    Device::Stop();

                    labelMode.SetState("����", Color::BLACK, Color::GREEN);

                    Timer::SetOnceTask(TimerTask::ChangeColorOnLabelStop, 10000, []()
                    {
                        labelMode.SetState("����", Color::BLACK, Color::GRAY);
                    });
                }
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
