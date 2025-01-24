// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Font/Font.h"
#include "GUI/GovernorGUI.h"
#include "Display/Primitives_.h"
#include "Hardware/Keyboard/Keyboard_.h"
#include "Display/Display_.h"
#include "Display/Colors_.h"
#include "Utils/Math_.h"

#pragma warning(push, 0)
#undef CRC
#include "GUI/Application.h"

#define uint    unsigned int
#define int8    signed char
#define uint8   unsigned char
#define int16   signed short
#define uint16  unsigned short
#define uint    unsigned int
#define uchar   unsigned char
#define pString const char * const

#include <wx/display.h>
#pragma warning(pop)

#undef uint   
#undef int8   
#undef uint8  
#undef int16  
#undef uint16 
#undef uint   
#undef uchar  
#undef pString


using namespace Primitives;

namespace Display
{
    void InitHardware();

    // Здесь хранятся указатели на кнопки
    static wxButton *buttons[Key::Count] = { nullptr };

    static GovernorGUI *governor = nullptr;

    static bool needStartTimerLong = false;
    static bool needStopTimerLong = false;

    // Здесь имя нажатой кнопки
    static Key::E pressedKey = Key::None;

    // Контекст рисования
    wxMemoryDC memDC;

    static wxBitmap bitmap(Display::PHYSICAL_WIDTH, Display::PHYSICAL_HEIGHT);

    // Создаёт окно приложения. Возвращает хэндл виджета для отрисовки
    static void CreateFrame();

    // Установить размер и оптимальную позицию для окна приложения
    static void SetPositionAndSize(Frame *frame);

    // Получить разрешение максимального имеющегося в системе монитора
    static wxRect GetMaxDisplay();

    // Создаёт все кнопки
    static void CreateButtons(Frame *frame);

    // Создаёт одну кнопку
    static void CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size);

    class Screen : public wxPanel
    {
    public:
        Screen(wxWindow *parent) : wxPanel(parent, 320)
        {
            SetMinSize({ Display::PHYSICAL_WIDTH, Display::PHYSICAL_HEIGHT });
            SetDoubleBuffered(true);
            Bind(wxEVT_PAINT, &Screen::OnPaint, this);
        }

        void OnPaint(wxPaintEvent &)
        {
            wxPaintDC dc(this);
            dc.DrawBitmap(bitmap, 0, 0);
        }
    };

    static Screen *screen = nullptr;

    static wxBitmap *backgroundBMP = nullptr;
}


void Display::InitHardware()
{
    CreateFrame();

    Font::Set(TypeFont::GOSTAU16BOLD);

    Font::SetSpacing(2);

    static unsigned char buffer[Display::PHYSICAL_WIDTH * Display::PHYSICAL_HEIGHT * 3];

    unsigned char *pointer = buffer;

    Color color1 = Color::BACK;
    Color color2 = Color::BACK;

    for (int i = 0; i < Display::PHYSICAL_WIDTH * Display::PHYSICAL_HEIGHT; i += 2)
    {
        *pointer++ = color1.GetRED();
        *pointer++ = color1.GetGREEN();
        *pointer++ = color1.GetBLUE();

        *pointer++ = color2.GetRED();
        *pointer++ = color2.GetGREEN();
        *pointer++ = color2.GetBLUE();

        if (i % Display::PHYSICAL_WIDTH == 0)
        {
            Math::Swap(color1, color2);
        }
    }

    wxImage image;
    image.Create(Display::PHYSICAL_WIDTH, Display::PHYSICAL_HEIGHT, buffer, true);

    static wxBitmap bmp(image);

    backgroundBMP = &bmp;
}


void Display::BeginScene(int, int)
{
    memDC.SelectObject(bitmap);
    wxBrush brush({ 0, 0, 0 }, wxTRANSPARENT);
    memDC.SetBrush(brush);

    memDC.DrawBitmap(*backgroundBMP, 0, 0);
}


void Display::EndScene()
{
    memDC.SelectObject(wxNullBitmap);
    screen->Refresh();
}


void Frame::OnTimerLong(wxTimerEvent &)
{
}


void Frame::HandlerEvents()
{
}


void Display::CreateFrame()
{
    Frame *frame = new Frame("");

    SetPositionAndSize(frame);

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

    screen = new Screen(frame);

    sizer->Add(screen);

    frame->SetSizer(sizer);

    CreateButtons(frame);

    frame->Show(true);
}


void Display::CreateButtons(Frame *frame)
{
    int x0 = 500;
    int y0 = 15;

    int dX = 10;
    int dY = 5;

    int w = 65;
    int h = 25;

    wxSize size(w, h);

    const int NUM_COLS = 5;
    const int NUM_ROWS = 4;

    static const Key::E keys[NUM_ROWS][NUM_COLS] =
    {
        { Key::_1,    Key::_2, Key::_3,  Key::Left,  Key::Right },
        { Key::_4,    Key::_5, Key::_6,  Key::None,  Key::OK    },
        { Key::_7,    Key::_8, Key::_9,  Key::None,  Key::Esc   },
        { Key::Minus, Key::_0, Key::Dot, Key::Start, Key::Stop  }
    };

    for (int row = 0; row < NUM_ROWS; row++)
    {
        for (int col = 0; col < NUM_COLS; col++)
        {
            int x = x0 + (w + dX) * col;
            int y = y0 + (h + dY) * row;

            Key::E key = keys[row][col];

            if (key == Key::OK || key == Key::Esc)
            {
                x -= (size.GetWidth() / 2 + dX / 2);
            }

            CreateButton(key, frame, { x, y }, size);
        }
    }

    governor = new GovernorGUI(frame, { 700, 170 });
}


void Display::SetPositionAndSize(Frame *frame)
{
    wxSize size = { Display::PHYSICAL_WIDTH + 400, Display::PHYSICAL_HEIGHT };

    frame->SetClientSize(size);
    frame->SetMinClientSize(size);
    frame->SetMaxClientSize(size);

    wxRect rect = GetMaxDisplay();

    frame->SetPosition({ rect.width / 2 - size.GetWidth() / 2, rect.height / 2 - size.GetHeight() / 2 });
}


void Display::CreateButton(Key::E key, Frame *frame, const wxPoint &pos, const wxSize &size)
{
    if (key == Key::None)
    {
        return;
    }

    wxButton *button = new wxButton(frame, (wxWindowID)key, Key::Name(key), pos, size);

    button->Connect((wxWindowID)key, wxEVT_LEFT_DOWN, wxCommandEventHandler(Frame::OnDown));
    button->Connect((wxWindowID)key, wxEVT_LEFT_UP, wxCommandEventHandler(Frame::OnUp));

    buttons[key] = button;
}


wxRect Display::GetMaxDisplay()
{
    wxRect result = { 0, 0, 0, 0 };

    for (uint i = 0; i < wxDisplay::GetCount(); i++)
    {
        wxDisplay display(i);

        wxRect rect = display.GetClientArea();
        if (rect.width > result.width)
        {
            result.width = rect.width;
            result.height = rect.height;
        }
    }

    return result;
}


void Display::Sender::SendToFSMC(int, int)
{

}
