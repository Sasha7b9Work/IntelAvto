// 2025/04/07 10:05:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Keyboard/Keyboard_.h"


struct InputField
{
    InputField();

    // ���� visible == true, �� ��������� ����� �����������
    void Draw(int x, int y, bool visible);

    void OnKey(Key::E);

    void Reset();

    // ���������� �������� � �����-��������
    int GetValueMilliUnits() const;

private:

    static const int SIZE_BUFFER = 6;
    char buffer[SIZE_BUFFER];

    bool IsConsist(char) const;
};
