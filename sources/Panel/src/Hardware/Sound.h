// 2025/02/11 13:45:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"


struct TypeWave
{
    enum E
    {
        Sine,
        Meandr,
        Triangle
    };
};


namespace Sound
{
    void Init();

    void Stop();

    // ���� ������� �� ������.
    void ButtonPress();

    // ������� ������� ���� ����������� ������ ������ ���� ����� ���� ������������ ���� ������� ������.
    void ButtonRelease();

    void GovernorChangedValue();

    void RegulatorShiftRotate();

    void RegulatorSwitchRotate();

    void WarnBeepBad();

    void WarnBeepGood();

    // ��� ������� ���� �������� ����� �������/��������� ����. ���� ����������� � ����.
    void WaitForCompletion();

    extern void *handle;
};
