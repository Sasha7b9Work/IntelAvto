// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"


class Item;


class Hint
{
public:

    // ������ ��������� ��� item
    static void Create(const Item *item);

    // ���������� ����� ���������
    static String Text();

    // true, ���� ��������� ��������
    static bool Shown();

    // ���������� ����, ��� �������� ��������� ���������
    static const Item *UnderItem();

    // �������� ���������
    static void Hide();
};
