// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Hardware/Timer.h"
#include "Display/Display_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "FlashDrive/FlashDrive.h"
#include "LAN/LAN.h"
#include "Device/IT6523.h"
#include "VCP/VCP.h"
#include "Hardware/Beeper.h"
#include "SCPI/SCPI.h"
#include "Device/Device.h"


#ifndef WIN32
    #if __ARMCC_VERSION != 6210000
        // �� ������ ������� ������������ �� �����������
        // �� �� 6.23 �� Keil 5.42a �� ����������� ��-�� new, malloc
        #error "Requires ARM Compiler V6.21 from uVision 5.39"
    #endif
#endif



int main(void)
{
    HAL::Init();

    gset.Load();

    LAN::Init();
    
    Display::Init();

    Keyboard::Init();

    Menu::Init();

    VCP::Init();

    FDrive::Init();

    Beeper::Init();
    
    while (1)
    {
        Device::TasksUpdate();
        Menu::Input::Update();

        Device::TasksUpdate();
        Display::Update();

        Device::TasksUpdate();
        Timer::UpdateTasks();

        Device::TasksUpdate();
        IT6523::Update();

        Device::TasksUpdate();
        FDrive::Update();

        Device::TasksUpdate();
        SCPI::Update();

        gset.Save();
    }
}
