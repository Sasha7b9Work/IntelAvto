// 2023/12/22 14:56:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Value
{
    Value(int _munits = 0) : munits(_munits) { }

    int munits;     // Значение в миллиюнитах (например, 1 = 1 мс)
};
