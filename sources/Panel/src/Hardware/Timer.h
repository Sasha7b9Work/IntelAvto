// 2024/02/05 11:50:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL.h"


#define TIME_MS HAL_TIM::TimeMS()


struct TimerTask
{
    enum E
    {
        ChangeColorOnLabelStop,         // Изменить цвет кнопки СТОП через некоторое время после остановки
        Count
    };
};


namespace Timer
{
    // Однократное значение - выполняется только один раз
    void SetOnceTask(TimerTask::E, uint time, void (*func)());

    void DisableTask(TimerTask::E);

    void UpdateTasks();
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
