#pragma once
#include <cstring>


template<int MAX_SIZE, class T = uint8 >
class Buffer
{
public:

    Buffer() : size(0) {} //-V730

    void Fill(T value)
    {
        for (int i = 0; i < size; i++)
        {
            buffer[i] = value;
        }
    }

    T *Data() { return buffer; }

    // Возвращает количество элементов в буфере
    int Size() const
    {
        return size;
    }

    int Capacity() const
    {
        return MAX_SIZE;
    }

    void RemoveFirst(int num_elements)
    {
        if (num_elements > size)
        {
            LOG_ERROR("Too many elements");
            num_elements = size;
        }

        if (num_elements == size)
        {
            size = 0;
        }
        else
        {
            size -= num_elements;
            std::memmove(buffer, buffer + num_elements, (uint)(size));
        }
    }

    void Append(T value)
    {
        if (Size() + 1 > Capacity())
        {
            LOG_ERROR("Buffer is full");
        }
        else
        {
            buffer[size++] = value;
        }
    }

    T &operator[](uint i)
    {
        if ((int)i >= 0 && (int)i < Capacity())
        {
            return buffer[i];
        }

        static T null(0);

        return null;
    }

protected:

    int size;

    T buffer[MAX_SIZE];
};


class Buffer2048 : public Buffer<2048>
{
public:
    Buffer2048() : Buffer<2048>() { }
};


// !!! ВНИМАНИЕ !!! Этот буфер берёт память из кучи.
class BufferHeap
{
public:
    BufferHeap(int size = 0);
    ~BufferHeap();
    void Realloc(int size);
    void Fill(uint8 value);
    uint8 *data;
    int Size() const
    {
        return size;
    }
    char *DataChar()
    {
        return reinterpret_cast<char *>(data);
    }
private:
    int size;
    void Free();
    void Malloc(int size);
};
