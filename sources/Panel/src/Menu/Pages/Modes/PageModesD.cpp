// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Menu/MenuItemsDef.h"
#include "Menu/Pages/Channels/Channels.h"


using namespace Primitives;


extern Item *items[7];


static void OnChanged_TypeMeasure()
{

}


DEF_SWITCH_2(sTypeMeasure,
    "Измерения", "Measure",
    "Выбор измерения", "Measurement selection",
    "Частота",   "Частота",
    "Frequency", "Frequency",
    Channel::D->mod.typeMeasure, OnChanged_TypeMeasure
)


static void OnChanged_ModeMeasureFrequency()
{

}


DEF_SWITCH_2(sModeFrequency,
    "Режим", "Mode",
    "Измерение частоы", "Pulse counting",
    "Частота",   "Частота",
    "Frequency", "Frequency",
    Channel::D->mod.modeFrequency, OnChanged_ModeMeasureFrequency
)

Switch *switchModeFrequencyD = &sModeFrequency;

static Item *items[7] =
{
    &sTypeMeasure,
    &sModeFrequency,
    ModesChannel::switchTimeMeasue,
    nullptr,
    nullptr
};

PageModes pageModesD(items, nullptr);
