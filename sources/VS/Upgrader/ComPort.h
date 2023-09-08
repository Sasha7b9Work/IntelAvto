// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct ComPort
{
    static bool Open();
    static void Close();
    static bool IsOpened();
    static void Send(pchar);
    static void Send(void *buffer, int size);
    static int Receive(void *buffer, int size, int timeWait);
};
