// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Transceiver.h"
#include "Hardware/HAL/HAL.h"


void Transceiver::Transmit(BaseMessage *message)
{
    HAL_SPI1::Transmit(message->Size(), 10);

    HAL_SPI1::Transmit(message->TakeData(), message->Size(), (message->Size() > 100) ? 200 : 10);
}
