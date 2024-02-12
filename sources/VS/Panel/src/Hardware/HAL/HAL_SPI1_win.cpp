// 2024/02/06 10:32:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"


void HAL_SPI1::CS(bool)
{

}


void HAL_SPI1::Transmit(const void * /*buffer*/, int /*size*/)
{
}


void HAL_SPI1::TransmitUInt(uint /*value*/)
{
}


uint HAL_SPI1::ReceiveUInt()
{
    return 0;
}
