// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/FreqMeter.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Pages/Channels/Channels.h"


#define TYPE_SYNCH(ch)      (glob_set.typeSynch[NUMBER_CHANNEL(ch)])
#define TYPE_SYNCH_A        (TYPE_SYNCH(Channel::A))
#define TYPE_SYNCH_B        (TYPE_SYNCH(Channel::B))

#define LEVEL_SYNCH(ch)     glob_set.levelSynch[NUMBER_CHANNEL(ch)][TYPE_SYNCH(ch)]
#define LEVEL_SYNCH_A       (LEVEL_SYNCH(Channel::A))
#define LEVEL_SYNCH_B       (LEVEL_SYNCH(Channel::B))

#define LANG_IS_RU          (glob_set.language == Language::RU)


struct LevelSynch
{
    // �������� ������� ������������� �� �������� ���������� ���������� (�������� �� �����������. �� ����������� ������ ��� ������ �� �������)
    static void Change(int delta);
    static void Set(int num);
};


// ����� ��� - ��� ���������� ������
struct StyleGUI
{
    enum E
    {
        Modern,     // ������� �������
        Classic     // ���������� �������
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
    int          levelSynch[4][2];  // ������� �������������
    TypeSynch::E typeSynch[4];      // ��� ������������� ��� ������� �� �������
    bool         showStatistics;    // ���������� �� ���������� �� FPS
    StyleGUI     styleGUI;
    Language::E  language;
    uint8        colorScheme;

    void Save();
    void Load();
    void Reset();
};

extern Settings glob_set;
