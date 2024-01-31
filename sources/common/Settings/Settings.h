// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"
#include "Menu/Value.h"


struct VoltageMode
{
    enum E
    {
        _12,
        _24,
        Count
    };

    static E Current();

    static bool Is12() { return Current() == _12; }

    static void Draw();
};


struct TypeSignal
{
    enum E
    {
        _1,     // Int
        _2a,    // Int
        _2b,    // Ext
        _3a,    // Int
        _3b,    // Int
        _4,     // Ext
        _5a,    // Ext
        _5b,    // Ext
        Count
    };

    static pchar Name();

    static void Set(E v) { current = v; }

    static bool Is(E v) { return current == v; }

    static pchar ToString();

    static E Current() { return current; }

private:

    static E current;
};


#define LANG_IS_RU          (gset.language == Language::RU)


// Стиль ГУИ - как отображать симолы
struct StyleGUI
{
    enum E
    {
        Modern,     // Обычные символы
        Classic     // Сегментные символы
    } value;

    StyleGUI(E v) : value(v) {}

    bool IsModern() const { return value == Modern; }
    void Switch() { value = (value == Modern) ? Classic : Modern; }
};


struct Language
{
    enum E
    {
        RU,
        EN
    };
};


struct SettingsSignal
{
    static const int MAX_PARAMS = 6;

    Value values12[MAX_PARAMS];
    Value values24[MAX_PARAMS];
};


struct Settings
{
    uint           size;
    ColorScheme    schemes[ColorScheme::COUNT];
    StyleGUI       styleGUI;
    Language::E    language;
    uint8          colorScheme;
    TypeSignal::E  signal;
    VoltageMode::E voltage_mode;
    SettingsSignal signals[TypeSignal::Count];

    void Save();
    void Load();
    void Reset();
};

extern Settings gset;
