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

    Primitives::Label labelMode(10, 170, "СТОП", Color::BLACK, Color::GREEN, Color::WHITE);
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
    if (!Device::IsStopped())                                               // Когда идёт тест,
    {
        if (control.key != Key::Start && control.key != Key::Stop)          // то обрабатываем только СТАРТ и СТОП
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
                        labelMode.SetState("ТЕСТ", Color::WHITE, Color::RED);

                        Device::Start();
                    }
                    else if (Device::IsRunning())
                    {
                        labelMode.SetState("ПАУЗА", Color::BLACK, Color::YELLOW);

                        Device::Pause();
                    }
                }
                else if (control.key == Key::Stop)
                {
                    Device::Stop();

                    labelMode.SetState("СТОП", Color::BLACK, Color::GREEN);

                    Timer::SetOnceTask(TimerTask::ChangeColorOnLabelStop, 10000, []()
                    {
                        labelMode.SetState("СТОП", Color::BLACK, Color::GRAY);
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
