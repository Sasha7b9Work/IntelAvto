// 2023/09/08 20:52:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Font/Font.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Device/Device.h"
#include "Hardware/Timer.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include <cstring>


namespace Menu
{
    // Текущая отображаемая страница меню
    Page *openedPage = nullptr;

    void (*funcUpdate)() = nullptr;

    Primitives::Label labelMode(100, 5, "СТОП", Color::BLACK, Color::GREEN, Color::WHITE);
}


void Menu::Draw()
{
//    openedPage->Draw(0, 0, 0);

//    if (Page::IsSignal(OpenedPage()))
//    {
//        labelMode.Draw();
//    }
}


Page *Menu::OpenedPage()
{
    return openedPage;
}


void Menu::SetOpenedPage(Page *page)
{
    openedPage = page;

    Menu::Input::SetFuncUpdate(Menu::Input::FuncUpdate);
}


void Menu::Input::OnKey(Key::E key)
{
    if (!Device::IsStopped())                                           // Когда идёт тест,
    {
        if (key != Key::Start && key != Key::Stop)                      // то обрабатываем только СТАРТ и СТОП
        {
            return;
        }
    }

//    if (!openedPage->SelectedItem()->OnKey(key))                        // Сначала пытаемся обработать тенущий элемент меню
//    {
//        if (!openedPage->OnKey(key))                                    // Потом передаём событие странице
//        {
//            if (key == Key::Start)
//            {
//                if (OpenedPageIsSignal())
//                {
//                    Timer::DisableTask(TimerTask::ChangeColorOnLabelStop);
//
//                    if (Device::IsStopped())
//                    {
//                        if (Device::Start())
//                        {
//                            labelMode.SetState("ТЕСТ", Color::WHITE, Color::RED);
//                        }
//                    }
//                    else if (Device::IsRunning())
//                    {
//                        labelMode.SetState("ПАУЗА", Color::BLACK, Color::YELLOW);
//
//                        Device::Pause();
//                    }
//                    else if (Device::InPause())
//                    {
//                        labelMode.SetState("ТЕСТ", Color::WHITE, Color::RED);
//
//                        Device::Resume();
//                    }
//                }
//            }
//            else if (key == Key::Stop)
//            {
//                if (OpenedPageIsSignal())
//                {
//                    Device::Stop();
//
//                    labelMode.SetState("СТОП", Color::BLACK, Color::GREEN);
//
//                    Timer::SetDefferedOnceTask(TimerTask::ChangeColorOnLabelStop, 10000, []()
//                    {
//                        labelMode.SetState("СТОП", Color::BLACK, Color::GRAY);
//                    });
//                }
//            }
//            else if (key == Key::Esc)
//            {
//                if (OpenedPageIsSignal())
//                {
////                    SetOpenedPage(PageMain::self);
//                }
//                else
//                {
////                    SetOpenedPage(PageSignal1::self);
//                }
//            }
//        }
//    }
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
    while (!Keyboard::Empty())
    {
        OnKey(Keyboard::NextKey());
    }
}


void Menu::Input::FuncEmptyUpdate()
{
    while (!Keyboard::Empty())
    {
        Keyboard::NextKey();
    }
}


void Menu::Input::SetFuncUpdate(void (*_funcUpdate)())
{
    funcUpdate = _funcUpdate;
}


bool Menu::OpenedPageIsSignal()
{
    Page *page = OpenedPage();

    return false;
}
