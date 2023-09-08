// 2022/01/18 16:01:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "Hardware/HAL/HAL.h"


static pchar FuncErase(pchar);
static pchar FuncWrite(pchar);


const StructSCPI SCPI::upgrade[] =
{
    SCPI_LEAF(":ERASE", FuncErase),
    SCPI_LEAF(":WRITE", FuncWrite), // :upgrade:write num_zone hash size. После этого идёт буфер байт размером size байт
    SCPI_EMPTY()
};


static pchar FuncErase(pchar buffer)
{
    if (SCPI::SU::IsLineEnding(&buffer))
    {
        SCPI::Answer::SendBadSymbols();

        HAL_EEPROM::EraseSector(HAL_EEPROM::ADDR_SECTOR_UPGRADE);

        SCPI::SendAnswer(":UPGRADE:ERASE");

        return buffer;
    }

    return nullptr;
}


static pchar FuncWrite(pchar)
{


    return nullptr;
}
