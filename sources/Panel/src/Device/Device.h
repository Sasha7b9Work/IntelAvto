// 2024/02/01 10:15:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Device
{
    // �� ������� �� START ��������
    void _Start();

    // �� ������� �� STOP ��������
    void Stop();

    // ���� � ��������� ����������
    bool IsRunning();

    // ���� � �����
    bool IsPause();

    // ���� �� �����������
    bool IsStopped();
}
