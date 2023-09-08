// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/Process/DataSender.h"


int SCPI::DataSender::valuesForSend = 0;
SCPI::DataSender::Mode::E SCPI::DataSender::mode = SCPI::DataSender::Mode::Hand;


void SCPI::DataSender::SendValue(pchar value)
{
    if (mode == Mode::Auto || valuesForSend > 0)
    {
        SendAnswer(value);

        if (valuesForSend > 0)
        {
            valuesForSend--;
        }
    }
}
