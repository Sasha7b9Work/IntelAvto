// 2024/02/02 00:22:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Value_.h"


// ���������� ������ ��������� ����������
namespace MCP4811
{
    void SetVoltage(const Value &);

    void Disable();

    void Test();
}
