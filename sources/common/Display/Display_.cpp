// 2023/09/08 20:56:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Device/Device.h"
#include "Display/Display_.h"
#include "Display/Font/Font.h"
#include "Display/Text_.h"
#include "Display/Console_.h"
#include "LAN/LAN.h"
#include "Hardware/Timer.h"
#include "Device/IT6523.h"
#include "Display/Primitives_.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


using namespace Primitives;


namespace Display
{
    Value value_in(0, TypeValue::Voltage);
    Value value_out(0, TypeValue::Voltage);

    int num_sends = 0;
    uint crc_trans = 0;
    uint crc_recv = 0;

    static WarningMessage::E warn = WarningMessage::Count;
    static uint time_warn = 0;

    static void SetTopRow(int i);
    static int topRow = 0;
    static uint timeStart = 0;
    static uint timeFrame = 0;

    static int topDraw = 0;             // Верхний у отрисовываемой части экрана
    static int bottomhDraw = 0;         // Нижний у отрисовываемой части экрана

    int width = PHYSICAL_WIDTH;
    int height = PHYSICAL_HEIGHT;

    static int yString = 110;

    static Coord coordMemory(15, yString);
    static Coord coordTest(40, yString);
    static Coord coordExtGenerator(95, yString);
    static Coord coordLaunch(130, yString);

    bool drawingScene = false;  // Если true - находимся в состоянии отрисовки основной части - между BeginScene()
                                // и EndScene()

    bool sendToSCPI = false;    // Если true, то надо посылать в SCPI

    static void DrawSignal();

    static void DrawScreen();

    void DrawRectangle1(int x, int y);

    void InitHardware();

    static void DisableWarningMessage();
}

void Display::Init()
{
    InitHardware();

    Font::Set(TypeFont::GOSTAU16BOLD);

    Font::SetSpacing(2);
}


void Display::DrawRectangle1(int x, int y)
{
    static uint time_start = TIME_MS;

    const int w = Display::PHYSICAL_WIDTH - x * 2;
    const int h = Display::PHYSICAL_HEIGHT - y * 2;
    const int x_start = x;
    const int x_end = x + w;
    const int y_start = y;
    const int y_end = y + h;

    int cell = 10;

    Rect(w, h).Draw(x, y);
    Rect(w - cell * 2, h - cell * 2).Draw(x + cell, y + cell);

    VLine vline(cell);
    HLine hline(cell);

    int delta = (int)((float)(TIME_MS - time_start) / 5.0f);

    while (delta > cell)
    {
        delta -= cell;
    }

    for (int i = x_start + delta; i < x_end; i += cell)
    {
        vline.Draw(i, y);
    }

    for (int i = y_start + delta; i < y_end; i += cell)
    {
        hline.Draw(x_end - cell, i);
    }

    for (int i = x_end - delta; i >= x_start; i -= cell)
    {
        vline.Draw(i, y_end - cell);
    }

    for (int i = y_end - delta; i >= y_start; i -= cell)
    {
        hline.Draw(x, i);
    }
}


void DrawRectangle2(int x, int y)
{
    int cell = 15;

    while (x >= 0)
    {
        int w = Display::PHYSICAL_WIDTH - x * 2;
        int h = Display::PHYSICAL_HEIGHT - y * 2;

        Rect(w, h).Draw(x, y);

        x -= cell;
        y -= cell;
    }

    Rect(Display::PHYSICAL_WIDTH, Display::PHYSICAL_HEIGHT).Draw(0, 0);
}


void DrawRectangle3(int, int)
{
    Rect(Display::PHYSICAL_WIDTH, Display::PHYSICAL_HEIGHT).Draw(0, 0);

    static uint time_start = TIME_MS;

    int cell = 20;

    Rect(Display::PHYSICAL_WIDTH - cell * 2, Display::PHYSICAL_HEIGHT - cell * 2).Draw(cell, cell);

    int delta = ((int)(TIME_MS - time_start) / 20) % cell;

    VLine vline(cell);
    HLine hline(cell);

    int x_start = 0;
    int x_end = Display::PHYSICAL_WIDTH;
    int y = 0;
    int y_start = 0;
    int y_end = Display::PHYSICAL_HEIGHT;
    int x = 0;

    for (int i = x_start + delta; i < x_end; i += cell)
    {
        vline.Draw(i, y);
    }

    for (int i = y_start + delta; i < y_end; i += cell)
    {
        hline.Draw(x_end - cell, i);
    }

    for (int i = x_end - delta; i >= x_start; i -= cell)
    {
        vline.Draw(i, y_end - cell);
    }

    for (int i = y_end - delta; i >= y_start; i -= cell)
    {
        hline.Draw(x, i);
    }
}


void DrawRectangle(int, int)
{
    Rect(Display::PHYSICAL_WIDTH, Display::PHYSICAL_HEIGHT).Draw(0, 0);

    static uint time_start = TIME_MS;

    int cell = 18;

    Rect(Display::PHYSICAL_WIDTH - cell * 2, Display::PHYSICAL_HEIGHT - cell * 2).Draw(cell, cell);

    const int delta = ((int)(TIME_MS - time_start) / 40) % cell;

    VLine vline(cell);
    HLine hline(cell);

    const int x_start = 0;
    const int x_end = Display::PHYSICAL_WIDTH;
    const int y_start = 0;
    const int y_end = Display::PHYSICAL_HEIGHT;

    for (int i = x_start + delta; i < x_end; i += cell)
    {
        DLine(cell, 1, 1).Draw(i, y_start);
    }

    for (int i = y_end - delta - 8; i >= y_start; i -= cell)
    {
        DLine(cell, 1, 1).Draw(x_end - cell, i);
    }

    for (int i = x_start + delta + 1; i < x_end + cell; i += cell)
    {
        DLine(cell, -1, -1).Draw(i, y_end - 1);
    }

    for (int i = y_end - delta - 2; i >= y_start; i -= cell)
    {
        Line().Draw(x_start, i, x_start + cell, i + cell);
    }
}


void Display::DrawWelcomeScreen()
{
    uint startTime = TIME_MS;

    while (TIME_MS - startTime < 5000)
    {
        for (int i = 0; i < NUM_PARTS; i++)
        {
            SetTopRow(i);
            BeginScene();
            Color::FILL.SetAsCurrent();

//            Text(String(LANG_IS_RU ? "OAO МНИПИ" : "OAO MNIPI")).WriteScaled(160, 70, 2);
//            Text(String(STR_NUM_VERSION)).WriteScaled(140, 152, 2);

            Text("OAO МНИПИ").WriteScaled(110, 60, 3);
            Text(STR_NUM_VERSION).WriteScaled(140, 160, 2);

            EndScene();
        }
    }
}

void Display::DrawKeyboardFailScreen()
{
    uint startTime = TIME_MS;

    while (TIME_MS - startTime < 3000)
    {
        for (int i = 0; i < NUM_PARTS; i++)
        {
            SetTopRow(i);
            BeginScene();
            Text("ОШИБКА КЛАВИАТУРЫ").Write(100, 110, Color::FILL);
            EndScene();
        }
    }
}

void Display::SetTopRow(int i)
{
    topRow = i * (Display::PHYSICAL_HEIGHT / Display::NUM_PARTS);
    topDraw = topRow;
    bottomhDraw = topDraw + Display::PHYSICAL_HEIGHT / Display::NUM_PARTS;
}


void Display::Update()
{
#ifdef GUI
    BeginScene();
    DrawScreen();
    Console::Draw();

    if (!TypeSignal::IsExtern())
    {
        Text("%.2f", value_in.ToUnits()).Write(400, 5, Color::WHITE);
        Text("%.2f", value_out.ToUnits()).Write(400, 30);
    }

    EndScene();
#else
    for (int i = 0; i < NUM_PARTS; i++)
    {
        LAN::Update();
        DrawPartScreen(i, true);
    }
#endif
}


void Display::DrawPartScreen(int num, bool)
{
    static int counter = 0;

    SetTopRow(num);

    if (num == 0)
    {
        timeStart = TIME_MS;
    }
    LAN::Update();

    Display::BeginScene();

    LAN::Update();

    DrawScreen();

    LAN::Update();

    if (num == 0)
    {
        Console::Draw();
    }

    if ((counter++) % 3)
    {
        Rect(5, 5).Fill(Display::PHYSICAL_WIDTH - 5, Display::PHYSICAL_HEIGHT - 5, Color::WHITE);
    }

    if (Device::IsRunning())
    {
        for (int i = 0; i < 5; i++)
        {
            Text("ТЕСТИРОВАНИЕ").Write(190, 80 + 20 * i, Color::WHITE);
        }
    }

    if (num_sends > 1)
    {
        Text("%d", num_sends).Write(420, 180, Color::WHITE);
        Text("%08X", crc_trans).Write(400, 210);
        Text("%08X", crc_recv).Write(400, 240);
    }

    LAN::Update();

    if (!TypeSignal::IsExtern())
    {
        Text("%.2f", (double)value_in.ToUnits()).Write(400, 5, Color::WHITE);
        Text("%.2f", (double)value_out.ToUnits()).Write(400, 30);
    }

    Display::EndScene();
    LAN::Update();

    if (num == Display::NUM_PARTS)
    {
        timeFrame = TIME_MS - timeStart;
    }
}


void Display::DrawScreen()
{
//    Menu::Draw();

//    if (Menu::OpenedPageIsSignal())
//    {
//        Text("Тип сигнала %s : %s",
//            TypeSignal::ToString(),
//            TypeSignal::Name()).Write(230, 6, Color::WHITE);
//
//        DrawSignal();
//    }
//    else if (Menu::OpenedPage()->ConsistOpenedItems())
//    {
//
//    }
//    else
    {
        Font::Set(TypeFont::GOSTB28B);

        const int y0 = 40;
        const int dy = 70;
        const int x = 150;
        const int w = 300;

        int y = y0;

        Text("ГЕНЕРАТОР").Write(x, y, w, Color::WHITE);

        y += dy;

        Text("ИМПУЛЬСОВ").Write(x, y, w);

        y += dy;

        Text("КПТС").Write(x, y, w);

        Font::Set(TypeFont::GOSTAU16BOLD);
    }

    if (warn != WarningMessage::Count)
    {
        if (IT6523::TimeLeftToHeavyImpulse() == 0)
        {
            DisableWarningMessage();
        }
        else
        {
            int w = 366;
            int h = 38;

            int x = 95;
            int y = 90;

            int d = 10;

            Rect(w, h).FillRounded(x, y, 2, Color::BACK, Color::WHITE);

            if ((((TIME_MS - time_warn) / 500) % 2) == 0)
            {
                Text("Импульс можно включить через %d секунд", IT6523::TimeLeftToHeavyImpulse()).
                    Write(x + d, y + d, Color::WHITE);
            }
        }
    }
}


int Display::TopRow()
{
    return topRow;
}


void Display::SendToSCPI()
{
    sendToSCPI = true;

    Update();
}


static int oldTopRow = 0;


void Display::Sender::Prepare(int w, int h)
{
    width = w;
    height = h;
    oldTopRow = topRow;
    topRow = 0;
}


void Display::Sender::Restore()
{
    width = PHYSICAL_WIDTH;
    height = PHYSICAL_HEIGHT;
    topRow = oldTopRow;
}


int Display::Width()
{
    return width;
}


int Display::Height()
{
    return height;
}


#ifdef WIN32

bool Display::InDrawingPart(int, int)
{
    return true;

#else

bool Display::InDrawingPart(int y, int _height)
{
    if (!drawingScene)
    {
        return true;
    }

    int yBottom = y + _height;

    if (y >= topDraw && y <= bottomhDraw)
    {
        return true;
    }

    if (y <= topDraw && yBottom >= bottomhDraw)
    {
        return true;
    }

    if (yBottom >= topDraw && yBottom <= bottomhDraw)
    {
        return true;
    }

    return false;

#endif

}


void Display::DrawSignal()
{
    Color::GRAY.SetAsCurrent();

//    Picture::DrawPicure(150, 50, TypeSignal::Current());

//    Page::ForCurrentSignal()->DrawParameters();
}


void Display::ShowWarningMessage(WarningMessage::E _warn)
{
    warn = _warn;

    time_warn = TIME_MS;

    Timer::SetDefferedOnceTask(TimerTask::DisplayWarningMessage, 4500, DisableWarningMessage);
}


void Display::DisableWarningMessage()
{
    warn = WarningMessage::Count;
}
