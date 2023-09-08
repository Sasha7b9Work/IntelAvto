// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Menu/MenuItemsDef.h"


// Выбор времени измерения
#ifdef USE_1000s

DEF_SWITCH_7(sTimeMeasure,
    "Время", "Время счета",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    "1ms", "10ms", "100ms", "1s", "10s", "100s", "1000s",
    ModesChannel::timeMeasure, TimeMeasure::LoadToFPGA
);

#else

DEF_SWITCH_6(sTimeMeasure,
    "Время", "Time",
    "Время счета", "Counting time",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    "1ms", "10ms", "100ms", "1s", "10s", "100s",
    ModesChannel::timeMeasure, TimeMeasure::LoadToFPGA
);

#endif


Switch *ModesChannel::switchTimeMeasue = &sTimeMeasure;


// Выбор числа усредняемых периодов входного сигнала
#ifdef USE_1000K

DEF_SWITCH_7(sNumberPeriods,
    "N", "Число периодов измерения",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    "1", "10", "100", "1K", "10K", "100K", "1000K",
    ModesChannel::numberPeriods, NumberPeriods::LoadToFPGA
);

#else

DEF_SWITCH_6(sNumberPeriods,
    "N", "N",
    "Число периодов измерения", "Number of measurement periods",
    "1", "10", "100", "1K", "10K", "100K",
    "1", "10", "100", "1K", "10K", "100K",
    ModesChannel::numberPeriods, NumberPeriods::LoadToFPGA
);

#endif


Switch *ModesChannel::switchNumberPeriods = &sNumberPeriods;


// Выбор периода меток времени
DEF_SWITCH_6(sTimeLabels,
    "Метки", "Stamps",
    "Длительность временных меток", "Duration of timestamps",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    "10-3", "10-4", "10-5", "10-6", "10-7", "10-8",
    ModesChannel::timeLabels, PeriodTimeLabels::LoadToFPGA
);


Switch *ModesChannel::switchTimeLabels = &sTimeLabels;


DEF_SWITCH_2(sTimeComparator,
    "Время изм.", "Time meas.",
    "Выбор времени одного измерения компаратора", "Timing of one comparator measurement",
    "1s", "10s",
    "1s", "10s",
    ModesChannel::timeComparator, TimeComparator::LoadToFPGA
);


Switch *ModesChannel::switchTimeComparator = &sTimeComparator;
