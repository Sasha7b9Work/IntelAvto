// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Messages.h"
#include "Connector/Handlers_d.h"
#include "Generator/Generator.h"


void DHandlers::Processing(BaseMessage &message)
{
    message.ResetPointer();

    Command::E command = message.PopCommand();

    if (command == Command::START_2A)
    {
        Value Us = message.PopValue();
        Value t1 = message.PopValue();

        Generator::Start2A(Us, t1);
    }
}
