// 2025/08/19 11:37:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "VCP/VCP.h"
#include "GUI/ConsoleSCPI.h"
#include <cstdarg>


void VCP::SendStringAsynch0D0A(char *format, ...)
{
    char buffer[200];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    std::strcat(buffer, "\r\n");
    ConsoleSCPI::Self()->AddText(buffer);
}


void VCP::SendStringAsynchRAW(char *format, ...)
{
    char buffer[200];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);
    ConsoleSCPI::Self()->AddText(buffer);
}
