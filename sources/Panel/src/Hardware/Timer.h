// 2024/02/05 11:50:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL.h"
#include "Connector/Device/Value_.h"


#define TIME_MS HAL_TIM::TimeMS()


class Parameter;


struct TimerTask
{
    enum E
    {
        ChangeColorOnLabelStop,         // �������� ���� ������ ���� ����� ��������� ����� ����� ���������
        USBD,
        StopSound,
        IT6523,
        Count
    };
};


namespace Timer
{
    // ����������� �������� - ����������� ������ ���� ���
    void SetDefferedOnceTask(TimerTask::E, uint time, void (*func)());

    void SetPeriodicTask(TimerTask::E, uint time, void (*func)());

    void DisableTask(TimerTask::E);

    void UpdateTasks();

    void Delay(uint timeMS);
}


struct TimeMeterMS
{
    TimeMeterMS()
    {
        Reset();
    }

    void Reset()
    {
        time_reset = TIME_MS;
    }

    uint ElapsedTime()
    {
        return TIME_MS - time_reset;
    }
private:
    uint time_reset;
};


// ����������� ����� �� ���������� ������ �����
namespace RemainingTimeCounter
{
    // ��������� ������� �������
    void Start(const Parameter &period, const Parameter &N);

    void Stop();

    // ���������� � ��������� ���� ���������� �����
    pchar RemainingTime(char buffer[128]);

    // ������������� ������� �������
    void Pause();

    // ���������� ������� �������
    void Resume();

    // ��� ������
    bool InProcess();
};
