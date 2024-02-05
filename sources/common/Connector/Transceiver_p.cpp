// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Transceiver.h"
#include "Display/Colors.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"


void Transceiver::Transmit(BaseMessage *message)
{
    int timeout = 10;

    bool result = false;

    while (!result)
    {
        HAL_SPI1::WaitRelease();                                                    // Ожидаем перехода флага готовности прибора в состояние "свободен"

        for (int i = 0; i < 2; i++)
        {
            HAL_SPI1::Transmit(message->Size(), 10);                                // Передаём размер передаваемых данных

            HAL_SPI1::Transmit(message->TakeData(), message->Size(), timeout);      // Передаём непосредственно данные

            uint newSize = 0;
            HAL_SPI1::Receive(&newSize, 4, 10);                                     // Теперь принимаем размер данных, которые хочет передать нам устройство

            uint trashedBytes = HAL_SPI1::ReceiveAndCompare(message->TakeData(), message->Size());

            result = (trashedBytes == 0);
        }
    }
}


bool Transceiver::Receive(BaseMessage *message)
{
    HAL_SPI1::WaitRelease();

    int size = 0;
    HAL_SPI1::Receive(&size, 4, 10);

    if (message->AllocateMemory(size))
    {
        HAL_SPI1::Receive(message->TakeData(), message->Size(), 50);

        return true;
    }

    return message->Size() != 0;
}
