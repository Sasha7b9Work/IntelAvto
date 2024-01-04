// 2023/09/08 16:18:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings.h"
#include "Utils/Math.h"
#include "Display/Colors.h"
#include "Hardware/HAL/HAL.h"


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
        Color::Make(127, 127, 127)
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
        Color::Make(127, 127, 127)
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
        Color::Make(127, 127, 127)
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
    {
//         Us       t1
        { Value(), Value() },                   // 1

//         Us       t1
        { Value(), Value() },                   // 2a

//         Us       td
        { Value(), Value() },                   // 2b

//         Us
        { Value() },                            // 3a

//         Us
        { Value() },                            // 3b

        {
            Value(Range(-6.0f, -7.0f),   Range(-12.0f, -16.0f)),    // 4 : Us
            Value(Range(-2.5f, -6.0f),   Range(-5.0f, -12.0f)),     // 4 : Ua
            Value(Range(14e-3f, 40e-3f), Range(50e-3f, 100e-3f)),   // 4 : t7
            Value(Range(0.5f, 20.0f),    Range(0.5f, 20.0f)),       // 4 : t9
            Value(Range(5e-3f, 100e-3f), Range(10e-3f, 100e-3f))    // 4 : t11
        },

//         Us       td
        { Value(), Value() },                   // 5a

//         Us       Uss      td
        { Value(), Value(), Value() }           // 5b
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


String TypeSignal::ToString()
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
