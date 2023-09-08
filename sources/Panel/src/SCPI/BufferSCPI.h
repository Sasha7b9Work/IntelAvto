// 2022/01/18 16:47:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
    Хранит принятые данные
*/

class BufferSCPI
{
public:
    BufferSCPI(int size = 0);
    ~BufferSCPI();
    void Append(uint8 *data, int size);
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
