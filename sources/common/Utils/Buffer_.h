// 2023/09/08 21:46:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
    ����� ����������� ������������ ��� ��������������� ���������/������������ ������ �� ����
*/

class Buffer
{
public:
    Buffer(int size = 0);
    ~Buffer();
    void Realloc(int size);
    void Fill(uint8 value);
    uint8 *data;
    int Size() const { return size; }
    char *DataChar() { return reinterpret_cast<char *>(data); }
private:
    int size;
    void Free();
    void Malloc(int size);
};
