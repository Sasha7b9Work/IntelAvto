// 2023/09/08 20:54:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
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


static void OnPress_TypeSignal()
{

}

// Выбор типа сигнала
DEF_SWITCH_3(sTypeSignal,
    "Сигнал", "Signal",
    "Время индикации", "Display time",
    "0.1s.", "1s.", "10s.",
    "0.1s.", "1s.", "10s.",
    PageIndication::displayTime, OnPress_TypeSignal
);


static void OnPress_RefGenerator()
{

}

// Выбор опорного генератора
DEF_SWITCH_2(sRefGenerator,
    "НП", "НП",
    "Выбор опорного генератора", "Selecting a reference generator",
    "Внутренний", "Внешний",
    "Internal",   "External",
    PageIndication::refGenerator, OnPress_RefGenerator
);


static void OnPress_LaunchSource()
{

}

// Выбор источника запуска
DEF_SWITCH_3(sLaunchSource,
    "Uист", "Uist",
    "Выбор источника запуска", "Selecting a trigger source",
    "Внутренний", "Внешний",  "Однократный",
    "Internal",   "External", "Single",
    PageIndication::launchSource, OnPress_LaunchSource);

static void OnPress_Calibration()
{
}

// Вход в режим калибровки
DEF_SWITCH_2(sCalibration,
    "Длит", "Dlit",
    "", "",
    "Не нажата",   "Нажата",
    "Not pressed", "Pressed",
    PageIndication::calibrationMode, OnPress_Calibration
);


static void OnPress_MemoryMode()
{

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
    "Амп", "Amp",
    "Режим память", "Memory mode",
    "Выкл", "Вкл",
    "Off",  "On",
    PageIndication::memoryMode, OnPress_MemoryMode
);

static Item *items[7] =
{
    &sTypeSignal,
    &sRefGenerator,
    &sLaunchSource,
    &sCalibration,
    &sMemoryMode,
    nullptr,
};

static Page pageIndication(items, nullptr, nullptr);

Page *PageIndication::self = &pageIndication;
