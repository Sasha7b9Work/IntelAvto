// 2023/09/08 21:15:38 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Primitives.h"
#include "Display/Font/Font.h"
#include "Display/Text.h"


using namespace Primitives;


void Rectangle::FillRounded(int x, int y, int round, Color colorFill, Color colorBound)
{
    DrawRounded(x, y, round, colorBound);
    Rectangle(width - 3, height - 3).Fill(x + 2, y + 2, colorFill);
    HLine(width - 3).Draw(x + 2, y + 1);
    HLine(width - 3).Draw(x + 2, y + height - 1);
    VLine(height - 3).Draw(x + 1, y + 2);
    VLine(height - 3).Draw(x + width - 1, y + 2);
}


void Rectangle::Fill(int x, int y, Color color)
{
    color.SetAsCurrent();

    Fill(x, y);
}


void Rectangle::DrawRounded(int x, int y, int round, Color color)
{
    color.SetAsCurrent();

    HLine(width - round * 2 + 1).Draw(x + round, y);
    HLine(width - round * 2 + 1).Draw(x + round, y + height);
    VLine(height - round * 2 + 1).Draw(x, y + round);
    VLine(height - round * 2 + 1).Draw(x + width, y + round);

    if (round == 2)
    {
        Point().Draw(x + 1, y + 1);
        Point().Draw(x + width - 1, y + 1);
        Point().Draw(x + 1, y + height - 1);
        Point().Draw(x + width - 1, y + height - 1);
    }
}


void Rectangle::Draw(int x, int y, Color color)
{
    color.SetAsCurrent();

    Draw(x, y);
}

void Rectangle::Draw(int x, int y)
{
    HLine(width).Draw(x, y);
    HLine(width).Draw(x, y + height - 1);
    VLine(height).Draw(x, y);
    VLine(height).Draw(x + width - 1, y);
}


void Rectangle::Fill(int x0, int y0)
{
    for (int y = y0; y < y0 + height; y++)
    {
        HLine(width).Draw(x0, y);
    }
}


void DLine::Draw(int x, int y)
{
    for (int i = 0; i < length; i++)
    {
        Point().Draw(x, y);
        x += dx;
        y += dy;
    }
}


void ArrowRight::Draw(int x, int y)
{
    int d = 3;

//    HLine(WIDTH).Draw(x, y);
    Line().Draw(x, y - d, x + WIDTH, y);
    Line().Draw(x, y + d, x + WIDTH, y);
}


void ArrowLeft::Draw(int x, int y)
{
    int d = 3;

//    HLine(WIDTH).Draw(x, y);
    Line().Draw(x, y, x + WIDTH, y + d);
    Line().Draw(x, y, x + WIDTH, y - d);
}


void ArrowUp::Draw(int x, int y)
{
    int d = 3;
    int height = 15;

    VLine(height).Draw(x, y);
    Line().Draw(x, y, x - d, y + height);
    Line().Draw(x, y, x + d, y + height);
}


void HMeasuringLines::Draw(const Coord &delta)
{
    int x0 = m_x0 + delta.x;
    int x1 = m_x1 + delta.x;
    int y0 = m_y0 + delta.y;
    int y1 = m_y1 + delta.y;
    int y2 = m_y2 + delta.y;

    VLine(y2 - y0).Draw(x0, y0);
    VLine(y2 - y0).Draw(x1, y0);
    HLine(x1 - x0).Draw(x0, y1);

    ArrowRight().Draw(x1 - ArrowRight::WIDTH, y1);
    ArrowLeft().Draw(x0, y1);

    int length_text = Font::GetLengthText(text);

    Text(text).Write((x0 + x1) / 2 - length_text / 2, y1 - Font::GetHeight());
}


void ExHMeasuringLines::Draw(const Coord &delta)
{
    int x0 = m_x0 + delta.x;
    int x1 = m_x1 + delta.x;
    int x2 = m_x2 + delta.x;
    int x3 = m_x3 + delta.x;
    int y0 = m_y0 + delta.y;
    int y1 = m_y1 + delta.y;
    int y2 = m_y2 + delta.y;

    VLine(y2 - y1).Draw(x1, y0);
    VLine(y2 - y1).Draw(x2, y0);
    HLine(x3 - x0).Draw(x0, y1);

    ArrowRight().Draw(x1 - ArrowRight::WIDTH, y1);
    ArrowLeft().Draw(x2, y1);

    Text(text).Write(x2 + d_text, y1 - Font::GetHeight());
}
