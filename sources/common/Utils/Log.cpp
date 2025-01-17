// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Log.h"
#include "Display/Console.h"
#include "Display/Text.h"


void Log::Message(pchar message)
{
    Console::AddString(message);
}


void Log::Message(pchar file, int line, pchar message)
{
    Console::AddString(Text("%s %d %s", file, line, message).c_str());
}
