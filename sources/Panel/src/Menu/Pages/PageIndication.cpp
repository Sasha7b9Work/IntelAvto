// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/FPGA.h"
#include "Hardware/FreqMeter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/MenuItemsDef.h"

using namespace Primitives;

extern Item *items[7];
static bool onceLaunch = false;

DisplayTime     PageIndication::displayTime(DisplayTime::_100ms);
RefGenerator    PageIndication::refGenerator(RefGenerator::Internal);
LaunchSource    PageIndication::launchSource(LaunchSource::Internal);
CalibrationMode PageIndication::calibrationMode(CalibrationMode::Disabled);
MemoryMode      PageIndication::memoryMode(MemoryMode::On);


static void OnPress_DisplayTime()
{
    FreqMeter::LoadDisplayTime();
}

// Выбор времени отображения результата измерения
DEF_SWITCH_3(sDisplayTime,
    "Т-ИНД", "T-IND",
    "Время индикации", "Display time",
    "0.1s.", "1s.", "10s.",
    "0.1s.", "1s.", "10s.",
    PageIndication::displayTime, OnPress_DisplayTime
);


static void OnPress_RefGenerator()
{
    RefGenerator::LoadToFPGA();
}

// Выбор опорного генератора
DEF_SWITCH_2(sRefGenerator,
    "ОГ", "REF",
    "Выбор опорного генератора", "Selecting a reference generator",
    "Внутренний", "Внешний",
    "Internal",   "External",
    PageIndication::refGenerator, OnPress_RefGenerator
);


static void OnPress_LaunchSource()
{
    LaunchSource::LoadToFPGA();
}

// Выбор источника запуска
DEF_SWITCH_3(sLaunchSource,
    "ЗАП", "START",
    "Выбор источника запуска", "Selecting a trigger source",
    "Внутренний", "Внешний",  "Однократный",
    "Internal",   "External", "Single",
    PageIndication::launchSource, OnPress_LaunchSource);

static void OnPress_Calibration()
{
    FreqMeter::LoadCalibration();

    if(PageIndication::calibrationMode.IsEnabled())
    {
        FPGA::ReadValueCalibrator();
    }
}

// Вход в режим калибровки
DEF_SWITCH_2(sCalibration,
    "Калибр", "Calibr",
    "", "",
    "Не нажата",   "Нажата",
    "Not pressed", "Pressed",
    PageIndication::calibrationMode, OnPress_Calibration
);


static void OnPress_MemoryMode()
{
    MemoryMode::LoadToFPGA();
}

bool PageIndication::OnceLaunch()
{
    return onceLaunch;
}

void PageIndication::OnceLaunchSwitchTrue()
{
    onceLaunch = true;
}

void PageIndication::OnceLaunchSwitchFalse()
{
    onceLaunch = false;
}

// Включение(отключение) режима памяти
DEF_SWITCH_2(sMemoryMode,
    "Память", "Memory",
    "Режим память", "Memory mode",
    "Выкл", "Вкл",
    "Off",  "On",
    PageIndication::memoryMode, OnPress_MemoryMode
);

static Item *items[7] =
{
    &sDisplayTime,
    &sRefGenerator,
    &sLaunchSource,
    &sCalibration,
    &sMemoryMode,
    nullptr,
};

static Page pageIndication(items, nullptr, nullptr);

Page *PageIndication::self = &pageIndication;
