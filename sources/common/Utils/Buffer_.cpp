// 2025/05/28 09:23:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Buffer_.h"
#include <cstdlib>


BufferHeap::BufferHeap(int s)
{
    Malloc(s);
}


BufferHeap::~BufferHeap()
{
    Free();
}


void BufferHeap::Realloc(int _size)
{
    Free();
    Malloc(_size);
}


void BufferHeap::Fill(uint8 value)
{
    if (size)
    {
        std::memset(data, value, static_cast<uint>(size));
    }
}


void BufferHeap::Free()
{
    std::free(data);
    data = nullptr;
    size = 0U;
}


void BufferHeap::Malloc(int s)
{
    if (s > 0)
    {
        data = static_cast<uint8 *>(std::malloc(static_cast<uint>(s)));
        size = (data) ? s : 0;

        if (!data)
        {
            //            LOG_ERROR("Нет памяти");
        }
    }
    else
    {
        data = nullptr;
        size = 0U;
    }
}

