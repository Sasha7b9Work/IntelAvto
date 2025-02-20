#pragma once
#include "defines.h"


namespace TCP
{
    void Init(void (*funcReciever)(pchar buffer, uint length));

    namespace Client
    {
        void SendBuffer(pchar buffer, uint length);

        // � ����� ������ ��������� "\r\n"
        void SendString(char *format, ...);

        // ������ ��������� "��� ����", ��� ���������� ����������� ��������
        void SendStringRAW(char *foramt, ...);

        bool IsConnected();
    }
}
