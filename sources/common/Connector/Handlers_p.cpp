// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Messages.h"
#include "Connector/Handlers_p.h"
#include "Display/Console.h"


namespace PHandlers
{
    static bool Request(BaseMessage *);
}


bool PHandlers::Processing(BaseMessage * /*msg*/)
{
    return true;
}
