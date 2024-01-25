// 2023/09/08 16:18:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings.h"
#include "Utils/Math.h"
#include "Display/Colors.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Text.h"


TypeSignal::E TypeSignal::current = TypeSignal::_1;

static ColorScheme def_colors[3] =
{
    {
        Color::Make(248, 248, 248),     // FILL             0
        Color::Make(0,   24,  0),       // BACK             1
        Color::Make(24,  24,  24),      // BACK_UNSELECT    2
        Color::Make(192, 192, 192),     // BACK_SELECT      3
        Color::Make(248, 0,   0),       // TYPE_RED         4
        Color::Make(0,   248, 0),       // TYPE_GREEN       5
        Color::Make(0,   0,   248),     // TYPE_BLUE        6
        Color::Make(248, 248, 248),     // WHITE            7
        Color::Make(0,   0,   0),       // EMPTY            8
        Color::Make(127, 127, 127)      // GRAY             9
    },
    {
        Color::Make(248, 248, 248),     // FILL             0
        Color::Make(0,   0,   0),       // BACK             1
        Color::Make(0,   56,  0),       // BACK_UNSELECT    2
        Color::Make(240, 160, 48),      // BACK_SELECT      3
        Color::Make(248, 0,   0),       // TYPE_RED         4
        Color::Make(0,   248, 0),       // TYPE_GREEN       5
        Color::Make(0,   0,   248),     // TYPE_BLUE        6
        Color::Make(248, 248, 248),     // WHITE            7
        Color::Make(112, 8,   8),       // EMPTY            8
        Color::Make(127, 127, 127)      // GRAY             9
    },
    {
        Color::Make(248, 248, 248),     // FILL             0
        Color::Make(0,   0,   88),      // BACK_1           1
        Color::Make(24,  24,  24),      // BACK_UNSELECT    2
        Color::Make(192, 192, 192),     // BACK_SELECT      3
        Color::Make(248, 0,   0),       // TYPE_RED         4
        Color::Make(0,   248, 0),       // TYPE_GREEN       5
        Color::Make(0,   0,   248),     // TYPE_BLUE        6
        Color::Make(248, 248, 248),     // WHITE            7
        Color::Make(0,   0,   0),       // EMPTY            8
        Color::Make(127, 127, 127)      // GRAY             9
    }
};


static Settings def_set =
{
    0,                                  // size
    {
        def_colors[0], def_colors[1], def_colors[2]
    },
    StyleGUI(StyleGUI::Modern),
    Language::RU,
    0,                                          // colorScheme
    TypeSignal::_1,
    VoltageMode::_12,
    {
        {   {   Voltage(-80000),    // 1 : Us 12
                Time(1000)    }     // 1 : t1 12
            ,
            {   Voltage(-500000),   // 1 : Us 24
                Time(2000)    }}    // 1 : Us 24
        ,
        {   {   Voltage(0),         // 2a : Us 12
                Time(0)    }        // 2a : t1 12
            ,
            {   Voltage(0),         // 2a : Us 24
                Time(0)    }}       // 2a : t1 24
        ,
        {   {   Voltage(0),         // 2b : Us 12
                Time(0)   }         // 2b : td 12
            ,
            {   Voltage(0),         // 2b : Us 24
                Time(0)   }}        // 2b : td 24
        ,
        {   {   Voltage(0) }        // 3a : Us 12
            ,
            {   Voltage(0) }}       // 3a : Us 24
        ,
        {   {   Voltage(0) }        // 3b : Us 12
            ,
            {   Voltage(0) }}       // 3b : Us 24
        ,
        {   {   Voltage(-6500),     // 4 : Us
                Voltage(-4000),     // 4 : Ua
                Time(20),           // 4 : t7
                Time(10000),        // 4 : t9
                Time(50)        }   // 4 : t11
            ,
            {   Voltage(-6500),     // 4 : Us
                Voltage(-4000),     // 4 : Ua
                Time(20),           // 4 : t7
                Time(10000),        // 4 : t9
                Time(50)        }}  // 4 : t11
        ,
        {   {   Voltage(0),         // 5a : Us 12
                Time(0)     }       // 5a : td 12
            ,
            {   Voltage(0),         // 5a : Us 24
                Time(0)     }}      // 5a : td 24
        ,
        {  {    Voltage(0),         // 5b : Us 12
                Voltage(0),         // 5b : Uss 12
                Time(0)     }       // 5b : td 12
            ,
            {   Voltage(0),         // 5b : Us 24
                Voltage(0),         // 5b : Uss 24
                Time(0)    }}       // 5b : td 24
    }
};


Settings gset = def_set;


void Settings::Load()
{
    HAL_EEPROM::LoadSettings(this);
}


void Settings::Save()
{
    HAL_EEPROM::SaveSettings(this);
}


void Settings::Reset()
{
    gset.schemes[gset.colorScheme] = def_colors[gset.colorScheme];
}


pchar TypeSignal::ToString()
{
    static const pchar values[Count] =
    {
        "1",
        "2a",
        "2b",
        "3a",
        "3b",
        "4",
        "5a",
        "5b"
    };

    return values[current];
}


pchar TypeSignal::Name()
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

    return names[current];
}


VoltageMode::E VoltageMode::Current()
{
    return gset.voltage_mode;
}
