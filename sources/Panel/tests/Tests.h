// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Tests
{
    void Run();

    void ValuesFPGA();

    void ValuesSTRICT();

    void FailExit();

    namespace Utils
    {
        // ���������� ��������� ����� � ������������ ���������
        double RandDouble();

        bool EqualsDouble(double d1, double d2);
    }
}

