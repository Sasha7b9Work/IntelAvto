// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Messages_.h"
#include "Utils/Queue.h"


namespace DInterface
{
    void Update();

    // ���������� ������ �� ������� ���������, ��������� ��������
    Queue &GetOutbox();
};
