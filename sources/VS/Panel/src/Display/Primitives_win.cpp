// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Primitives_.h"


#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


using namespace Primitives;


namespace Display
{
    extern wxMemoryDC memDC;
}


int HLine::Draw(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    Draw(x, y);

    return x + length;
}


int HLine::Draw(const Coord &coord, const Color &color)
{
    return Draw(coord.x, coord.y, color);
}


int HLine::Draw(int x, int y)
{
    Display::memDC.DrawLine({ x, y }, { x + length, y });

    return x + length;
}


int HLine::Draw(const Coord &coord)
{
    return Draw(coord.x, coord.y);
}


int VLine::Draw(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    return Draw(x, y);
}


int VLine::Draw(int x, int y)
{
    Display::memDC.DrawLine({ x, y }, { x, y + length });

    return y + length;
}


int VLine::Draw(const Coord &coord)
{
    return Draw(coord.x, coord.y);
}


Coord Line::Draw(int x1, int y1, int x2, int y2)
{
    Display::memDC.DrawLine({ x1, y1 }, { x2, y2 });

    return { x2, y2 };
}


Coord Line::Draw(const Coord &coord, int x2, int y2)
{
    return Draw(coord.x, coord.y, x2, y2);
}


void Point::Draw(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    Draw(x, y);
}


void Point::Draw(int x, int y)
{
    m_x = x;
    m_y = y;

    Display::memDC.DrawPoint({ x, y });
}

