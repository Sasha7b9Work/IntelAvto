// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Console
{
    static void Init();

    static void Draw();

    static void AddString(pchar string);
    // ���������� true, ���� ������� ������ ������������ �� ������
    static bool IsShown();
    // ��� ������� ���� �������, ����� ������������ �������� ������������ ���������� ����� � �������
    static void OnChanged_MaxStringsInConsole();

private:
    static char buffer[10][100];
    // true ��������, ��� ��� ������� ������ ������� � ��������� � �� ����� ������ ������ (��� ����������, ����� ���������� ��� �� ����������)
    static bool inProcessDraw;
    // true, ���� ���������� ������� ���������� ������
    static bool inProcessAddingString;
    // ���������� ����������� ����� � �������
    static int stringInConsole;
    // ����� ����������� ���������� �������� ������������� ���������� ����� � �������
    static int16 prevMaxStrinsInConsole;

    static void DeleteFirstString();
    // ���������� true, ���� ������� ������ � � ��� ������ ����������� ������
    static bool IsBusy();
};

