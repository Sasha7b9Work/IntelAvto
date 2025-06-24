// 2025/02/27 10:37:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"


namespace IT6523
{
    void Update();

    // ��������� ������� ������������ ������������� ���������, ����������� ������� �����������
    // ���������� false, ���� ������ �� ����� ���� ��������� - �� ������ ���������� �������
    bool Start(TypeSignal::E, int num_pulses);

    // �������� �������������� ���������� �� ������� ����������
    bool Start(TypeSignal::E);

    // ������������� ������� ������������ ���������
    void Pause();

    // ���������� ������� ������������ ���������
    void Resume();

    // ���������� ������� ������������ ���������
    void Stop();

    // ���� �������� ������������ ���������
    void SendCommand(pchar);

    void SendCommandF(pchar, ...);

    // ����� ���������� ��������� �������
    void CallbackOnTimerImpulse();

    // ������� ��������� �������� ����������
    int RemainedPulses();

    // �������� �� ������� ���������� "�������" ��������
    uint TimeLeftToHeavyImpulse();
}
