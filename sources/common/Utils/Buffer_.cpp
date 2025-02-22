// 2023/09/08 21:46:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Buffer_.h"
#include <cstdlib>
#include <cstring>



Buffer::Buffer(int s)
{
    Malloc(s);
}


Buffer::~Buffer()
{
    Free();
}


void Buffer::Realloc(int _size)
{
    Free();
    Malloc(_size);
}


void Buffer::Fill(uint8 value)
{
    if (size)
    {
        std::memset(data, value, static_cast<uint>(size));
    }
}


void Buffer::Free()
{
    std::free(data);
    data = nullptr;
    size = 0U;
}


void Buffer::Malloc(int s)
{
    if (s > 0)
    {
        data = static_cast<uint8 *>(std::malloc(static_cast<uint>(s)));
        size = (data) ? s : 0;

        if(!data)
        {
//            LOG_ERROR("��� ������");
        }
    }
    else
    {
        data = nullptr;
        size = 0U;
    }
}
