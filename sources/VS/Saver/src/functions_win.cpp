// 2022/1/15 16:14:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/FPGA.h"
#include "Menu/Pages/Channels/Channels.h"


Channel *Channel::current = nullptr;

Channel *Channel::A = nullptr;
Channel *Channel::B = nullptr;


void HAL_EEPROM::LoadSettings(Settings *)
{

}


void HAL_EEPROM::SaveSettings(Settings *)
{

}


void FPGA::WriteCommand(const Command &)
{

}


void FPGA::GovernorData::IncreaseN()
{

}


void FPGA::GovernorData::DecreaseN()
{

}


void FPGA::GovernorData::SetN(int)
{

}


void FPGA::GovernorData::Write()
{

}
