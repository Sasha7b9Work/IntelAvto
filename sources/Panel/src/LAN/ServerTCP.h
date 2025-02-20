// 2025/02/20 11:45:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
    Подключается к удалённому серверу
*/

namespace ServerTCP
{
    void Init(void (*callbackReceive)(pchar, uint));

    void SendString(pchar buffer);

    void SendBuffer(pchar buffer, uint length);

    bool IsConnected();
}
