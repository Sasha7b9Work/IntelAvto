// 2024/02/01 10:54:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Value.h"


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
private:
    // ���������� ������������ ����� ��� �������. ����� ���������� ���� ��������� �
    // ����� � ������� buffer[0] ����� ��������� ���������� ���� ��� ��������
    uint &Pointer() { return buffer[0]; }
    static const int SIZE = 16;
    uint buffer[SIZE];
};


struct BaseMessage
{
    BaseMessage(Command::E command)
    {
        buffer.Push(command);
    }

    void Push(const Value &value)
    {
        buffer.Push(value);
    }

    Value PopValue()
    {
        return buffer.PopValue();
    }

    void Send() const;

    // ����������� �� ������� �������
    virtual void Execute() { }

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
