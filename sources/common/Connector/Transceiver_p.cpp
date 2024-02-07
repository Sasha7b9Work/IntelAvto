// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Transceiver.h"
#include "Display/Colors.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"


void Transceiver::Transmit(BaseMessage *message)
{
    bool result = false;

    while (!result)
    {
        HAL_SPI1::WaitRelease();                                                    // Ожидаем перехода флага готовности прибора в состояние "свободен"

        for (int i = 0; i < 2; i++)
        {
            HAL_SPI1::Transmit(message->Size());                                // Передаём размер передаваемых данных

            HAL_SPI1::Transmit(message->TakeData(), message->Size());      // Передаём непосредственно данные

            uint newSize = 0;
            HAL_SPI1::Receive(&newSize, 4);                                     // Теперь принимаем размер данных, которые хочет передать нам устройство

            uint trashedBytes = HAL_SPI1::ReceiveAndCompare(message->TakeData(), message->Size());

            result = (trashedBytes == 0);
        }
    }
}
