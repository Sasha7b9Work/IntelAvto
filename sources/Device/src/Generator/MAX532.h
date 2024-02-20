// 2024/02/04 18:33:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Value.h"


// Источник питания истокового повторителя
namespace MAX532
{
    void Set(const Value &);

    void Test();
}
