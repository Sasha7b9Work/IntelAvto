// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace SCPI
{

    // ��� �������� ������ � VCP
    struct DataSender
    {
        struct Mode
        {
            enum E
            {
                Hand,
                Auto,
                Count
            };
        };

        // ����� ���������� ���������� ����������� �������� ValueFPGA ������ �������� ��� �������
        static void SendValue(pchar data);

        // ��������� ������� ��� ��������������� ������ ������� ������
        static void SetMode(Mode::E m) { mode = m; };

        // ����������� ������� � ���� n ��������� ��������
        static void SendArray(int n)   { valuesForSend = n; };

    private:

        static int valuesForSend;

        static Mode::E mode;
    };
}
