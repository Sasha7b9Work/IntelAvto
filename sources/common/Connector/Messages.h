// 2024/02/01 10:54:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Value.h"
#include <cstring>


struct Command
{
    enum E
    {
        START_1,
        START_2A,
        START_2B,
        START_3A,
        START_3B,
        START_4,
        START_5A,
        START_5B,
        STOP,
        Count
    };
};


struct BufferMessage
{
    BufferMessage()
    {
        Pointer() = 1;
    }
    void Push(Command::E command)
    {
        if (Pointer() < SIZE)
        {
            buffer[Pointer()++] = command;
        }
    }
    void Push(const Value &value)
    {
        if (Pointer() < SIZE)
        {
            buffer[Pointer()++] = (uint)value.GetRaw();
        }
    }
    Value PopValue()
    {
        return Value((int)buffer[Pointer()++]);
    }
    int Size()
    {
        return (int)(sizeof(buffer[0]) * Pointer());
    }
    uint8 *TakeData()
    {
        return (uint8 *)buffer;
    }
    bool operator==(BufferMessage &rhs)
    {
        int size = Size();

        if (size != rhs.Size())
        {
            return false;
        }

        return std::memcmp(TakeData(), rhs.TakeData(), (uint)size) == 0;
    }
private:
    // Индексация производится через эту функцию. После погружения всех элементов в
    // буфер в элеенте buffer[0] будет храниться количество слов для передачи
    uint &Pointer() { return buffer[0]; }
    static const int SIZE = 16;
    uint buffer[SIZE];
};


struct BaseMessage
{
    BaseMessage(Command::E command = Command::Count)
    {
        buffer.Push(command);
    }
    virtual ~BaseMessage() { }

    void Push(const Value &value)
    {
        buffer.Push(value);
    }

    Value PopValue()
    {
        return buffer.PopValue();
    }

    void Send() const;

    int Size()
    {
        return buffer.Size();
    }

    uint8 *TakeData()
    {
        return buffer.TakeData();
    }

    bool AllocateMemory(int)
    {
        return true;
    }

    // Выполняется на приёмной староне
    virtual void Execute() { }

    bool IsEquals(BaseMessage *rhs)
    {
        return buffer == rhs->buffer;
    }

    BaseMessage *Clone();

    void Transmit();

    void TransmitAndSend();

private:

    BufferMessage buffer;
};


namespace Message
{
    struct Start2A : public BaseMessage
    {
        Start2A(const Value &Us, const Value &t1) : BaseMessage(Command::START_2A)
        {
            Push(Us);
            Push(t1);
        }

        virtual void Execute() override;
    };
}
