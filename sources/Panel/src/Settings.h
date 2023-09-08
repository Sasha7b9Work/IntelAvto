// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


#define LANG_IS_RU          (glob_set.language == Language::RU)


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


struct Settings
{
    uint         size;
    ColorScheme  schemes[ColorScheme::COUNT];
    StyleGUI     styleGUI;
    Language::E  language;
    uint8        colorScheme;

    void Save();
    void Load();
    void Reset();
};

extern Settings glob_set;
