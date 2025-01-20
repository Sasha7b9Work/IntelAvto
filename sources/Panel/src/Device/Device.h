// 2024/02/01 10:15:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Device
{
    // �� ������� �� ����� ��������
    void Start();

    // ��� ���� ���������� ��� ������� �� �����, �� ����� ���� � �������� ����������
    void Pause();

    // ��� ���������� ��� ������� �� �����, ����� ��������� � �����
    void Resume();

    // �� ������� �� ���� ��������
    void Stop();

    // ���� � ��������� ����������
    bool IsRunning();

    // ���� � �����
    bool InPause();

    // ���� �� �����������
    bool IsStopped();
}
