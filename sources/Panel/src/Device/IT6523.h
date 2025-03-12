// 2025/02/27 10:37:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"


namespace IT6523
{
    void Update();

    // Запускает процесс формирования периодических импульсов
    void Start(TypeSignal::E, int num_pulses);

    // Приостановить процесс формирования импульсов
    void Pause();

    // Продолжить процесс формирования импульсов
    void Resume();

    // Остановить процесс формирования импульсов
    void Stop();

    // Само дополнит завершающими символами
    void SendCommand(pchar);

    void SendCommandF(pchar, ...);

    // Здесь происходит очередной импульс
    void CallbackOnTimerImpulse();
}
