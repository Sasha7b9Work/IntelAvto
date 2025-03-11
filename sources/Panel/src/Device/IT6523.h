// 2025/02/27 10:37:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace IT6523
{
    void Update();

    void Start();

    void Pause();

    void Resume();

    void Stop();

    // Само дополнит завершающими символами
    void SendCommand(pchar);

    void SendCommandF(pchar, ...);
}
