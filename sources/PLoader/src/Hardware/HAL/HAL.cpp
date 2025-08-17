#include "HAL.h"



__attribute__ ((noreturn)) void HAL::ErrorHandler(const char *, int)
{
    while (true) {}
}


void HAL::Init()
{
}
