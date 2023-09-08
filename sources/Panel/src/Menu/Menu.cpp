// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings.h"
#include "Calculate/ValueFPGA.h"
#include "Display/Display.h"
#include "Display/Font/Font.h"
#include "Keyboard/Keyboard.h"
#include "Hardware/FPGA.h"
#include "Menu/Hint.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Menu/Pages/Pages.h"
#include <cstring>


namespace Menu
{
    void SubscribeToEvents();

    // Текущая отображаемая страница меню
    Page *openedPage = nullptr;

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


void Menu::Input::OnGovernorRotate(const Control &control)
{
    if (Menu::OpenedPage() == PageTuneColors::self)
    {
        return;
    }

    if (CURRENT_CHANNEL_IS_A_OR_B && control.IsRotateGovernor())
    {
        int delta = Channel::Current()->set.typeSynch.IsHoldoff() ? 1 : 2;

        if (control.value == Control::GovLeft)
        {
            delta = -delta;
        }

        if (PageIndication::calibrationMode.IsEnabled())
        {
            if (control.value == Control::GovLeft)
            {
                FPGA::GovernorData::DecreaseN();
            }
            else
            {
                FPGA::GovernorData::IncreaseN();
            }
            
            FPGA::GovernorData::Write();
        }
        else
        {
            LevelSynch::Change(delta);
        }
    }
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
        PageService::self,    // Service,
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
        FPGA::GovernorData::Reset();

        PageIndication::calibrationMode.value = CalibrationMode::Disabled;

        FreqMeter::LoadCalibrationNoSave();
        
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
        Channel::Current()->PressSetup();
        openedPage = (openedPage == Channel::Current()->pageModes) ? Channel::Current()->pageSettings : 
                                                                     Channel::Current()->pageModes;
        Hint::Hide();
        return true;

    case Control::Channels:
        {
            bool openSettings = (Menu::OpenedPage() == Channel::Current()->pageSettings);
            bool openModes = (Menu::OpenedPage() == Channel::Current()->pageModes);
            Channel::SetCurrentNext();
            Channel::LoadCurrentToFPGA(openSettings, openModes);
        }
        return true;

    case Control::Test:
        if ((Channel::A->mod.modeFrequency.IsRatioAC() && CURRENT_CHANNEL_IS_A) ||
            (Channel::B->mod.modeFrequency.IsRatioBC() && CURRENT_CHANNEL_IS_B))
        {
        }
        else
        {
            FreqMeter::modeTest.Switch();
        }
        break;

    case Control::Auto:
        if ((Channel::A->mod.typeMeasure.IsFrequency() && Channel::A->mod.modeFrequency.IsFrequency() && CURRENT_CHANNEL_IS_A) ||
            (Channel::B->mod.typeMeasure.IsFrequency() && Channel::B->mod.modeFrequency.IsFrequency() && CURRENT_CHANNEL_IS_B) ||
            (Channel::A->mod.typeMeasure.IsPeriod() && Channel::A->mod.modePeriod.IsPeriod() && CURRENT_CHANNEL_IS_A) ||
            (Channel::B->mod.typeMeasure.IsPeriod() && Channel::B->mod.modePeriod.IsPeriod() && CURRENT_CHANNEL_IS_B) ||
            (Channel::A->mod.typeMeasure.IsDuration() && Channel::A->mod.modeDuration.IsNdt() && CURRENT_CHANNEL_IS_A) ||
            (Channel::B->mod.typeMeasure.IsDuration() && Channel::B->mod.modeDuration.IsNdt() && CURRENT_CHANNEL_IS_B))
        {
            FPGA::Auto::Refresh();
            FreqMeter::LoadAuto();
            FPGA::EnableAuto();
            Keyboard::Lock();
        }
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
            FreqMeter::LoadOneTime();

            return true;
        }
        else if ((CURRENT_CHANNEL_IS_A && Channel::A->mod.typeMeasure.IsCountPulse() && Channel::A->mod.modeCountPulse.Is_StartStop()) ||
            (CURRENT_CHANNEL_IS_B && Channel::B->mod.typeMeasure.IsCountPulse() && Channel::B->mod.modeCountPulse.Is_StartStop()))
        {
            ModeStartStop::Toggle();
            ModeStartStop::LoadToFPGA();

            return true;
        }
    }

    return false;
}


void Menu::Init()
{
    Input::SetFuncUpdate(Input::FuncUpdate);

    openedPage = Channel::A->pageModes;

    SubscribeToEvents();

    PageTuneColors::Init();
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
    FreqMeter::modeTest.AddObserver(Channel::A->pageModes);
    FreqMeter::modeTest.AddObserver(Channel::B->pageModes);
}
