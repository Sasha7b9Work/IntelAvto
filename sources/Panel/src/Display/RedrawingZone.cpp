// 2023/09/08 20:56:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings.h"
#include "Display/Display.h"
#include "Display/Indicator.h"
#include "Display/RedrawingZone.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Display/Font/Font.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageIndication.h"
#include "Utils/Math.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include "Hardware/HAL/HAL.h"
#include <cctype>
#include <cstdlib>
#include <cstring>


using namespace Primitives;


uint ProgressBarTimeMeasureZone::timeStart = 0;
uint SynchroZone::timeStart = 0;


void RedrawingZone::Update(RedrawingZone::ModeDraw::E mode)
{
    modeDraw = mode;

    x0 = (mode == ModeDraw::ToHardware) ? 0 : left;
    y0 = (mode == ModeDraw::ToHardware) ? 0 : top;

    if (mode == RedrawingZone::ModeDraw::ToBuffer)
    {
        if (Display::InDrawingPart(top, height))
        {
            Draw();
        }
    }
    else
    {
        if (needUpdate)
        {
            BeforeDraw();

            Display::Sender::Prepare(width, height);

            FillBackground();

            if (Draw())
            {
                needUpdate = false;
            }

            Display::Sender::SendToFSMC(left, top);

            Display::Sender::Restore();

            AfterDraw();
        }
    }
}


void RedrawingZone::Refresh()
{
    if (PageIndication::calibrationMode.IsEnabled())
    {
        return;
    }

    if (Menu::OpenedPage()->IsAddition())
    {
        return;
    }

    needUpdate = true;
}


void RedrawingZone::FillBackground()
{
    Display::BeginScene(left, top);
}


#define RIGHT_DIGITS 360
#define LEFT_UNTIS   380


bool DataZone::Draw()
{
    return true;
}


bool ProgressBarTimeMeasureZone::Draw()
{
    return true;
}


void ProgressBarTimeMeasureZone::Reset()
{
    timeStart = TIME_MS;
}

void SynchroZone::BeforeDraw()
{
    oldColor = Color::FILL.Value();
}


void SynchroZone::AfterDraw()
{
    Color::FILL.SetValue(oldColor);
}


bool SynchroZone::Draw()
{
    int size = CalculateSize();

    if (size > 0)
    {
        int d = (MAX_SIZE - size) / 2;

        Primitives::Rectangle(size, size).Fill(x0 + d, y0 + d, Color::FILL);
    }

    return true;
}


uint SynchroZone::CalculateColor()
{
    uint8 color = (uint8)((1.0F - CalculateRelativeTime() * CalculateRelativeTime()) * 0xFF);

    return Color::Make(color, color, color);
}


int SynchroZone::CalculateSize()
{
//    if (CalculateRelativeTime() < 1.0F)
//    {
//        return MAX_SIZE;
//    }
//
//    return 0;

    return (int)(MAX_SIZE * (1.0F - CalculateRelativeTime()));
}


float SynchroZone::CalculateRelativeTime()
{
    const float timeLife = 2000.0F;

    float time = (float)(TIME_MS - timeStart);

    LIMITATION_ABOVE(time, timeLife);

    return (time / timeLife);
}


void SynchroZone::Fire()
{
    timeStart = TIME_MS;
}
