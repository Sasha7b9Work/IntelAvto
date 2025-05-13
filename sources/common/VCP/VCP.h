// 2025/03/19 10:57:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <usbd_def.h>


namespace VCP
{
    void Init();

    void SendBufferSynch(const uint8 *data, int size);

    // ��� ������ ��������� � ������������ ��������� \\r\\n
    void SendStringAsynch(char *format, ...);

    // ������ ��������� "��� ����"
    void SendStringAsynchRAW(char *format, ...);

    void Flush();

    extern USBD_HandleTypeDef handleUSBD;

    extern PCD_HandleTypeDef handlePCD;

    extern bool isConnected;

    extern bool cableIsConnected;
};
