// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Messages.h"
#include "Utils/Queue.h"


namespace DInterface
{
    void Update();

    // ���������� ������ �� ������� ���������, ��������� ��������
    Queue &GetOutbox();

    void ResetFreqForSend();

    // ��������� ��������� � ������� ��������. ���� ������� ���������, ��������� ��������� � ������� ���������� false
    bool AddMessageForTransmit(BaseMessage *message);
};
