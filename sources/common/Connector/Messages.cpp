// 2024/02/01 10:54:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Messages.h"
#include <cstring>


BaseMessage *BaseMessage::Clone()
{
    BaseMessage *result = new BaseMessage();

    result->buffer = buffer;

    return result;
}


BaseMessage::BaseMessage(uint8 *data, int size)
{
    buffer.SetFromRAW(data, size);
}
