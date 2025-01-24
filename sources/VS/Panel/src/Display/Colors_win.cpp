// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Primitives_.h"

#pragma warning(push, 0)
#undef CRC
#include <wx/wx.h>
#pragma warning(pop)


using namespace Primitives;


namespace Display
{
    extern wxMemoryDC memDC;
}

static Color current = Color::BACK;



void Color::SetAsCurrent() const
{
    uint val = Value();
    uint8 r = (uint8)val;
    uint8 g = (uint8)(val >> 8);
    uint8 b = (uint8)(val >> 16);

    wxColour colorDraw = wxColour(r, g, b);

    Display::memDC.SetPen(wxPen(colorDraw));

    current = *this;
}


Color Color::GetCurrent()
{
    return current;
}
