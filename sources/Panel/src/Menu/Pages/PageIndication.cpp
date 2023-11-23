// 2023/09/08 20:54:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/MenuItemsDef.h"
#include "Settings.h"

using namespace Primitives;

extern Item *items[7];
static bool onceLaunch = false;

TypeSignal      PageIndication::typeSignal(TypeSignal::_1);
RefGenerator    PageIndication::refGenerator(RefGenerator::Internal);
LaunchSource    PageIndication::launchSource(LaunchSource::Internal);
CalibrationMode PageIndication::calibrationMode(CalibrationMode::Disabled);
MemoryMode      PageIndication::memoryMode(MemoryMode::On);


static void OnPress_TypeSignal()
{
}

// Выбор типа сигнала
DEF_SWITCH_8(sTypeSignal,
    "Сигнал", "Signal",
    "Время индикации", "Display time",
    "1", "2a", "2b", "3a", "3b", "4", "5a", "5b",
    "1", "2a", "2b", "3a", "3b", "4", "5a", "5b",
    PageIndication::typeSignal, OnPress_TypeSignal
);



static void OnPress_RefGenerator()
{

}

// Выбор опорного генератора
DEF_SWITCH_2(sRefGenerator,
    "Ua", "Ua",
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
    "Us", "Us",
    "Выбор источника запуска", "Selecting a trigger source",
    "Внутренний", "Внешний",  "Однократный",
    "Internal",   "External", "Single",
    PageIndication::launchSource, OnPress_LaunchSource);

static void OnPress_Calibration()
{
}

// Вход в режим калибровки
DEF_SWITCH_2(sCalibration,
    "t1", "t1",
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
    nullptr,
    nullptr,
};

static Page pageIndication(items, nullptr, nullptr);

Page *PageIndication::self = &pageIndication;


pchar TypeSignal::Name(E v)
{
    static const pchar names[Count] =
    {
        "1",
        "2a",
        "SAEJ1113-11",
        "3a",
        "3b",
        "DIN40839",
        "16750-1",
        "16750-1"
    };

    return names[v];
}
