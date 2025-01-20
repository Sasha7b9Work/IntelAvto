// 2024/02/01 10:54:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Value_.h"
#include "Utils/StringUtils.h"
#include <cstring>


struct Command
{
    enum E
    {
        START_1_12V,
        START_1_24V,
        START_2A,
        START_3A,
        START_3B,
        STOP,
        SET_VOLTAGE,
        Count
    };
};


struct BufferMessage
{
    BufferMessage()
    {
        ResetPointer();
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
        return Value(buffer[Pointer()++]);
    }
    Command::E PopCommand()
    {
        return (Command::E)buffer[Pointer()++];
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
    uint CalculateCRC()
    {
        return SU::CalculateHash(&buffer[0], (int)Pointer() * (int)sizeof(buffer[0]));
    }
    void ResetPointer()
    {
        Pointer() = 1;
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

    Command::E PopCommand()
    {
        return buffer.PopCommand();
    }

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

    void ResetPointer()
    {
        buffer.ResetPointer();
    }

    uint CalculateCRC()
    {
        return buffer.CalculateCRC();
    }

private:

    BufferMessage buffer;
};


namespace Message
{
    struct Stop : public BaseMessage
    {
        Stop() : BaseMessage(Command::STOP) { }
    };


    struct SetVoltage : public BaseMessage
    {
        SetVoltage(const Value &U) : BaseMessage(Command::SET_VOLTAGE)
        {
            Push(U);
        }
    };


    struct Start1_12V : public BaseMessage
    {
        Start1_12V(const Value &Us, const Value &t1) : BaseMessage(Command::START_1_12V)
        {
            Push(Us);   // Амплитуда
            Push(t1);   // Период повторения
        }
    };


    struct Start1_24V : public BaseMessage
    {
        Start1_24V(const Value &Us, const Value &t1) : BaseMessage(Command::START_1_24V)
        {
            Push(Us);   // Амплитуда
            Push(t1);   // Период повторения
        }
    };


    struct Start2A : public BaseMessage
    {
        Start2A(const Value &Us, const Value &t1) : BaseMessage(Command::START_2A)
        {
            Push(Us);
            Push(t1);
        }
    };


    struct Start3A : public BaseMessage
    {
        Start3A(const Value &Us) : BaseMessage(Command::START_3A)
        {
            Push(Us);
        }
    };


    struct Start3B : public BaseMessage
    {
        Start3B(const Value &Us) : BaseMessage(Command::START_3B)
        {
            Push(Us);
        }
    };
}
