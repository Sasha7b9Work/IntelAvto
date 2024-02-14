// 2024/02/01 23:52:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Value.h"


namespace Generator
{
    void Stop();

    void Start1(const Value &period, const Value &duration);

    void Start2A(const Value &Us, const Value &period, const Value &duration);
}
