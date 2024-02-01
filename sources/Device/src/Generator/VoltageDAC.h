// 2024/02/02 00:22:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Value.h"


namespace VoltageDAC
{
    // Запись в ЦАП кода, соотвествующего нулевому выходному напряжению
    void SetZero();

    // Установка выходного напряжения источника питания истокового повторителя
    void SetOutputSource(const Value &);

    // Установка выходного напряжения модуля высоковольтного источника
    void SetHiVoltage(const Value &);
}
