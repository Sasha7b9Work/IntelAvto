// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Messages.h"
#include "Connector/Handlers_p.h"
#include "Display/Console.h"


namespace PHandlers
{
    static bool Request(BaseMessage *);
}


static bool E(BaseMessage *)
{
    return false;
}


bool PHandlers::Processing(BaseMessage * /*msg*/)
{
    return true;
}


static bool PHandlers::Request(BaseMessage *)
{
    return false;
}
