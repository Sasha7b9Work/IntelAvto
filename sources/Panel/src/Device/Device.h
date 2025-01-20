// 2024/02/01 10:15:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Device
{
    // По нажатию на START вызываем
    void _Start();

    // По нажатию на STOP вызываем
    void Stop();

    // Тест в состоянии выполнения
    bool IsRunning();

    // Тест в паузе
    bool IsPause();

    // Тест не выполняется
    bool IsStopped();
}
