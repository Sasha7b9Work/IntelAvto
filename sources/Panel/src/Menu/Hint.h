// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"


class Item;


class Hint
{
public:

    // Создаёт подсказку для item
    static void Create(const Item *item);

    // Возвращает текст подсказки
    static String Text();

    // true, если подсказка показана
    static bool Shown();

    // Возвращает итем, для которого действует подсказка
    static const Item *UnderItem();

    // Спрятать подсказку
    static void Hide();
};
