// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "GUI/ComPort.h"
#include "Application_win.h"
#include "Display/Display_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include <ctime>
#include <cstring>
#include <cstdio>


void init()
{
    Display::Init();
    Keyboard::Init();
}


static void OpenPort()
{
    for (int i = 0; i < 10; i++)
    {
        ComPort::Open();
        if (ComPort::IsOpened())
        {
            break;
        }
    }
}


void update()
{
#define SIZE_FRAME (272 * 480)

    static uint displayFrame[SIZE_FRAME];

    if (Frame::Self()->needSave)
    {
        OpenPort();

        if (ComPort::IsOpened())
        {
            std::memset(displayFrame, 0, SIZE_FRAME * 4);

            ComPort::Send(":picture\x0d");

            printf("%d", ComPort::Receive((char *)displayFrame, SIZE_FRAME * 4, 20000));

            Display::Draw(displayFrame);

            Frame::Self()->needSave = false;

            ComPort::Close();
        }
    }
}
