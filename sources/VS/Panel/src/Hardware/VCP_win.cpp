// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/ConsoleSCPI.h"
#include "Hardware/VCP.h"


void VCP::SendStringAsynch(const char *message)
{
    ConsoleSCPI::Self()->AddText(ConsoleSCPI::REVERSE_PROMT);
    ConsoleSCPI::Self()->AddText(message);
}
