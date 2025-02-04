// 2025/02/04 09:21:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace TCP
{
    void Init(void (*funcReciever)(pchar buffer, uint length));

    void SendBuffer(pchar buffer, uint length);

    // � ����� ������ ��������� "\r\n"
    void SendString(char *format, ...);

    // ������ ��������� "��� ����", ��� ���������� ����������� ��������
    void SendStringRAW(char *foramt, ...);

    bool IsConnected();
}
