// 2025/02/04 09:21:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace LAN
{
    void Init();

    // ������� ����� ������������ �������/�������� ����� timeMS �����������. ��� ���� ��� ������� ������� �������� ����������. WARN ��������. ���� ���������.
    void Update(uint timeMS);

    extern bool cableIsConnected;
};
