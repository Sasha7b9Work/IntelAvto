// 2024/02/05 11:50:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Timer.h"
#include "Menu/MenuItems.h"
#include "Utils/StringUtils_.h"


namespace Timer
{
    struct StructTask
    {
        uint time_next_run;         // В это время должно сработать задание
        uint dT;
        bool repeat;                // Если true - бесконечное повторение
        void (*func)();

        StructTask(bool _repeat = false, uint _dT = 0, void (*_func)() = nullptr) :
            time_next_run(0), dT(_dT), repeat(_repeat), func(_func) { }
    };

    static StructTask tasks[TimerTask::Count];
}


namespace RemainingTimeCounter
{
    static uint time_end = (uint)-1;    // В это время заканчивается отсчёт
}


void Timer::SetOnceTask(TimerTask::E task, uint time, void (*func)())
{
    tasks[task].dT = time;
    tasks[task].time_next_run = TIME_MS + time;
    tasks[task].repeat = false;
    tasks[task].func = func;
}


void Timer::DisableTask(TimerTask::E task)
{
    tasks[task].func = nullptr;
}


void Timer::UpdateTasks()
{
    uint time = TIME_MS;

    for (int i = 0; i < TimerTask::Count; i++) //-V1008
    {
        StructTask &task = tasks[i];

        if (task.func)
        {
            if (time >= task.time_next_run)
            {
                task.func();

                if (task.repeat)
                {
                    task.time_next_run += task.dT;
                }
                else
                {
                    task.func = nullptr;
                }
            }
        }
    }
}


void RemainingTimeCounter::Start(const Parameter &period, const Parameter &N)
{
    time_end = TIME_MS + (uint)(period.GetValue().ToInt() * N.GetValue().ToInt());
}


pchar RemainingTimeCounter::RemainingTime(char buffer[32])
{
    if (TIME_MS >= time_end)
    {
        return SU::TimeMStoText(0, buffer);
    }

    return SU::TimeMStoText(time_end - TIME_MS, buffer);
}
