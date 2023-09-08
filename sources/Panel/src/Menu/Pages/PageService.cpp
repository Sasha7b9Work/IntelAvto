// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings.h"
#include "Menu/Menu.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Hint.h"
#include "Menu/Pages/Pages.h"


static void OnPress_Information()
{
    Menu::SetOpenedPage(PageInfo::self);
}

DEF_BUTTON(bInformation,
    "Инфомация", "Information",
    "Вывод системной информации", "Displaying system information",
    OnPress_Information);


static void OnPress_ResetSettings()
{
    TimeMeasure::Set(TimeMeasure::_1ms);
    NumberPeriods::Set(NumberPeriods::_1);
    PeriodTimeLabels::Set(PeriodTimeLabels::T_8);

    Channel::A->mod.typeMeasure.value = TypeMeasure::Frequency;
    Channel::A->mod.modeFrequency.value = ModeFrequency::Frequency;
    
    if(FreqMeter::modeTest.IsEnabled())
    {
        FreqMeter::modeTest.Switch();
    }
    LevelSynch::Set(0);
    
    Channel::SetCurrent(Channel::A);

    Menu::SetOpenedPage(Channel::A->pageModes);
    
    Channel::Current()->OnChanged_TypeMeasure();

    Channel::Current()->LoadToFPGA();

    Channel::Current()->OnChanged_TypeMeasure();

    Channel::Current()->LoadToFPGA();
    
    LevelSynch::Set(0);
    
    FreqMeter::HardwareReset();
}

DEF_BUTTON(bResetSettings,
    "Сброс", "Reset",
    "Сброс настроек в состояние по умолчанию", "Resetting the settings to the default state",
    OnPress_ResetSettings
);


static void OnPress_Language()
{
    glob_set.language = LANG_IS_RU ? Language::EN : Language::RU;
    Hint::Create(Menu::OpenedPage()->SelectedItem());
    glob_set.Save();
}

DEF_BUTTON(bLanguage,
    "Language", "Язык",
    "Выбор языка интерфейса", "Selecting the interface language",
    OnPress_Language
);


static void OnPress_Colors()
{
    Menu::SetOpenedPage(PageColors::self);
}


DEF_BUTTON(bColors,
    "Цвета", "Colors",
    "Настройка цветовой схемы", "Customizing the color scheme",
    OnPress_Colors
);


static void OnPress_ShowStatistics()
{
    glob_set.showStatistics = !glob_set.showStatistics;
}

DEF_BUTTON(bShowStatistics,
    "Статистика", "Statistics",
    "Показывать временную статистику", "Show temporary statistics",
    OnPress_ShowStatistics
);


static void OnPress_StyleGUI()
{
    glob_set.styleGUI.Switch();
}


DEF_BUTTON(bStyleGUI,
    "Интерфейс", "Interface",
    "Выбор интерфейса пользователя", "User interface selection",
    OnPress_StyleGUI
);


static Item *items[7] =
{
    &bInformation,
    &bResetSettings,
    &bColors,
    &bLanguage,
    nullptr,    // &bStyleGUI,
    nullptr,    // &bShowStatistics,
    nullptr
};

void PageService::UseReset()
{
    OnPress_ResetSettings();
}

static Page pageService(items, nullptr, nullptr);

Page *PageService::self = &pageService;
