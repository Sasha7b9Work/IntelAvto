// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Console_.h"
#include "Display/Text_.h"
#include "Utils/Log_.h"
#include <cstdio>


void Log::Message(pchar message)
{
    Console::AddString(message);

#ifdef WIN32

//    printf(message);
//    printf("\n");

#endif
}


void Log::Message(pchar file, int line, pchar message)
{
    Console::AddString(Text("%s %d %s", file, line, message).c_str());
}
