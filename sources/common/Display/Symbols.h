// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define SYMBOL_AC                  0x80
#define SYMBOL_AC_IN_QUOTES       "\x80"
#define SYMBOL_DC                  0x81
#define SYMBOL_DC_IN_QUOTES       "\x81"
#define SYMBOL_FRONT               0x82     // �����
#define SYMBOL_FRONT_IN_QUOTES    "\x82"
#define SYMBOL_RISE                0x83     // ����
#define SYMBOL_RISE_IN_QUOTES     "\x83"


struct UGO
{
    static pchar OVERLAPPED;    // ��������� ������������
    static pchar EMPTY_VALUE;   // ���������, ����� �������� ��� �� �������� ����� ������� ��������
    static pchar DivNULL;       // ��������� ������� �� ����
};