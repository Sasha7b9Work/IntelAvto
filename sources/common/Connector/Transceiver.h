// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Messages.h"


/*

    Передача должна производиться так.

*/


namespace Transceiver
{
    void Transmit(BaseMessage *message);

    bool Receive(BaseMessage *message);
};
