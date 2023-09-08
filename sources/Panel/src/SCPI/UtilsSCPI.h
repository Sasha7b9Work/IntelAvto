// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace SCPI
{
    struct SU
    {
        // ���������� true, ���� ��������� ��������� �� ����������� ������������������
        static bool IsLineEnding(pchar *bufer);

        // ���� ������ buffer ���������� � ������������������ �������� word, �� ���������� ��������� �� ������,
        // ��������� �� ��������� �������� ������������������ word. ����� ���������� nullptr.
        static pchar BeginWith(pchar buffer, pchar word);

        // ��������� ����� ����� �� ������� ����������� �������. ���� ���-�� �������, ���������� true, ������
        // ���������� false. � outEnd ���������� ��������� �� ������, ��������� �� ��������� ���������
        static bool ReadIntegerValue(pchar buffer, int *outValue, pchar *outEnd);
    };
}
