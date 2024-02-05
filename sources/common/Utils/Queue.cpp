// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Messages.h"
#include "Queue.h"


Queue::Queue() : first(0), last(0)
{
    storage[0] = nullptr;
}


void Queue::Pop()
{
    if (Size() > 0)
    {
        delete storage[first];
        first++;
        if (first == last)
        {
            first = 0;
            last = 0;
        }
    }
}


bool Queue::Push(BaseMessage *message)
{
    if (last < SIZE - 1)
    {
        storage[last] = message;
        last++;
        return true;
    }

    return false;
}


int Queue::Size()
{
    return last - first;
}


BaseMessage *Queue::Front()
{
    BaseMessage *result = nullptr;

    if (Size() > 0)
    {
        result = storage[first];
    }

    return result;
}
