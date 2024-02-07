// 2024/02/06 10:32:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"


bool HAL_SPI1::Transmit(const void * /*buffer*/, int /*size*/)
{
    return false;
}


bool HAL_SPI1::Transmit(int /*value*/)
{
    return false;
}


bool HAL_SPI1::Receive(void * /*recv*/, int /*size*/)
{
    return false;
}


uint HAL_SPI1::ReceiveAndCompare(const void * /*compared*/, int /*size*/)
{
    return 0;
}


bool HAL_SPI1::WaitRelease()
{
    return false;
}
