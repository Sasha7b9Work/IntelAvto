// 2022/11/23 12:38:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/Commands.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


namespace SCPI
{
    void Send(pchar);

    void Error(pchar);
}


bool SCPI::CommandWithParameters::Execute()
{
    return false;
}


bool SCPI::CommandIDN::Execute()
{
    String<> message("OAO MNIPI, KPTS, v1.0.1");
    Send(message.c_str());
    return true;
}


bool SCPI::CommandSignalGet::Execute()
{
    String<> message("SIGNAL 1");
    Send(message.c_str());
    return true;
}
