// 2023/09/08 20:56:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Pages.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


using namespace Primitives;


namespace Display
{
    // Нарисовать подсказку
    void DrawHint(int x, int y);

    void DrawInfo();

    static void SetTopRow(int i);


    uint timeAutoHint = 0;
    int second = 0;
    static int topRow = 0;


    static uint timeStart = 0;
    static uint timeFrame = 0;
    uint fps = 0;                // Столько кадров отрисовано за последнюю секунду
    uint beginSecond = 0;        // В это время началась последняя секунда
    uint timePaint = 0;          // Время отрисовки за секунду


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

    void DrawSignal();

    static void DrawScreen();

    void DrawRectangle1(int x, int y);

    void InitHardware();
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

    int counter = 0;

    while (TIME_MS - startTime < 3000)
    {
        for (int i = 0; i < NUM_PARTS; i++)
        {
            SetTopRow(i);
            BeginScene();
            Color::FILL.SetAsCurrent();

//            Text(String(LANG_IS_RU ? "OAO МНИПИ" : "OAO MNIPI")).WriteScaled(160, 70, 2);
//            Text(String(STR_NUM_VERSION)).WriteScaled(140, 152, 2);

            Text(String(LANG_IS_RU ? "OAO МНИПИ" : "OAO MNIPI")).WriteScaled(110, 60, 3);
            Text(String(STR_NUM_VERSION)).WriteScaled(140, 160, 2);

            EndScene();
        }

        counter++;
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
            Text(LANG_IS_RU ? "ОШИБКА КЛАВИАТУРЫ" : "ERROR KEYBOARD").Write(100, 110, Color::FILL);
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
    EndScene();
#else
    for (int i = 0; i < NUM_PARTS; i++)
    {
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

    Display::BeginScene();

    DrawScreen();

    if (num == 0)
    {
        Console::Draw();
    }

    if ((counter++) % 3)
    {
        Rect(10, 10).Fill(460, 260, Color::WHITE);
    }

    Display::EndScene();

    if (num == Display::NUM_PARTS)
    {
        timeFrame = TIME_MS - timeStart;
    }
}


void Display::DrawScreen()
{
    Menu::Draw();

    Text(String(LANG_IS_RU ? "Тип сигнала %s : %s" : "Type signal %s : %s",
        TypeSignal::ToString(),
        TypeSignal::Name())).Write(230, 6, Color::WHITE);

    DrawSignal();

    VoltageMode::Draw();
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
    int x = 130;
    int y = 70;
    Coord coord(x, y);

    Color::GRAY.SetAsCurrent();

    if (TypeSignal::Is(TypeSignal::_1))
    {
        Axis().Draw(Coord(x, y + 25), 70, 330, 150);

        Text("Ua").Write(x - 25, y - 10);

        HMeasuringLines(x + 70, x + 290, y - 30, y - 15, y + 25).Draw();

        VMeasuringLines(x + 70, x + 200, x + 210, y + 25, y + 175).Draw();

        VLine(25).Draw(coord);
        coord.x = HLine(50).Draw(coord, Color::WHITE);
        coord.y = VLine(25).Draw(coord);
        coord.x = HLine(20).Draw(coord);
        coord = Line().Draw(coord, coord.x + 20, coord.y + 150);
        coord = Line().Draw(coord, coord.x + 30, coord.y - 110);
        coord = Line().Draw(coord, coord.x + 50, coord.y - 40);
        coord.x = HLine(50).Draw(coord);
        VLine(25).Draw(coord.x, coord.y - 25);
        coord.x = HLine(50).Draw(coord.x, coord.y - 25);
        coord.y = VLine(25).Draw(coord.x, coord.y - 25);
        coord.x = HLine(20).Draw(coord);
    }
    else if (TypeSignal::Is(TypeSignal::_2a))
    {
        Axis().Draw(Coord(x, y + 170), 210, 330, 10);

        Text("Ua").Write(x - 25, y + 140);

        HMeasuringLines(x + 50, x + 245, y - 30, y - 15, y + 151).Draw();

        VMeasuringLines(x + 70, x + 180, x + 200, y, y + 150).Draw();

        coord.y += 150;

        coord.x = HLine(50).Draw(coord, Color::WHITE);
        coord = Line().Draw(coord, coord.x + 15, coord.y - 140);
        coord = Line().Draw(coord, coord.x + 10, coord.y - 10);
        coord.x = HLine(10).Draw(coord);
        coord = Line().Draw(coord, coord.x + 10, coord.y + 10);
        coord = Line().Draw(coord, coord.x + 50, coord.y + 140);
        coord.x = HLine(100).Draw(coord);
        coord = Line().Draw(coord, coord.x + 15, coord.y - 140);
    }
    else if (TypeSignal::Is(TypeSignal::_2b))
    {
        Axis().Draw(Coord(x, y + 170), 210, 330, 10);

        Text("Ua").Write(x - 25, y + 10);

        HMeasuringLines(x + 70, x + 270, y - 5, y + 10, y + 170).Draw();

        VMeasuringLines(x + 90, x + 290, x + 310, y + 45, y + 170).Draw();

        Point().Draw(x, y + 20, Color::WHITE);
        Point::MoveOn(30, 0);
        Point::MoveOn(30, 150);
        Point::MoveOn(10, 0);
        Point::MoveOn(20, -120);
        Point::MoveOn(5, -5);
        Point::MoveOn(10, 0);
        Point::MoveOn(5, 5);
        Point::MoveOn(160, 120);
    }
    else if (TypeSignal::Is(TypeSignal::_3a))
    {
        Axis().Draw(Coord(x, y + 10), 55, 330, 60);

        VMeasuringLines(x + 140, x + 170, x + 220, y - 10, y + 60).Draw();

//        VMeasuringLines(x + 160, x + 250, x + 270, y + 90, y + 176).Draw();

        Text("Ua").Write(x - 25, y - 15);

        Point().Draw(x, y - 10, Color::WHITE);
        Point::MoveOn(20, 0);

        for (int i = 0; i < 6; i++)
        {
            Point::MoveOn(0, 70);
            Point::MoveOn(5, 0);
            Point::MoveOn(0, -70);
            Point::MoveOn(5, 0);
        }

        Point::MoveOn(130, 0);

        for (int i = 0; i < 6; i++)
        {
            Point::MoveOn(0, 70);
            Point::MoveOn(5, 0);
            Point::MoveOn(0, -70);
            Point::MoveOn(5, 0);
        }

        Point().Draw(x + 90, y + 90);
        Point::MoveOn(50, 0);
        Point::MoveOn(15, 80);
        Point::MoveOn(5, 5);
        Point::MoveOn(5, 0);
        Point::MoveOn(5, -5);
        Point::MoveOn(15, -80);
        Point::MoveOn(50, 0);
    }
    else if (TypeSignal::Is(TypeSignal::_3b))
    {
        Axis().Draw(Coord(x, y + 170), 100, 330, 00);

        VMeasuringLines(x + 160, x + 210, x + 220, y - 35, y + 50).Draw();

//        VMeasuringLines(x + 110, x + 160, x + 220, y + 80, y + 150).Draw();

        Text("Ua").Write(x - 25, y + 140);

        Point().Draw(x, y + 150, Color::WHITE);
        Point::MoveOn(20, 0);

        for (int i = 0; i < 6; i++)
        {
            Point::MoveOn(0, -70);
            Point::MoveOn(3, 30);
            Point::MoveOn(8, 25);
            Point::MoveOn(9, 15);
        }

        Point::MoveOn(70, 0);

        for (int i = 0; i < 4; i++)
        {
            Point::MoveOn(0, -70);
            Point::MoveOn(3, 30);
            Point::MoveOn(8, 25);
            Point::MoveOn(9, 15);
        }

        Point().Draw(x + 90, y + 50);
        Point::MoveOn(50, 0);
        Point::MoveOn(15, -80);
        Point::MoveOn(5, -5);
        Point::MoveOn(5, 0);
        Point::MoveOn(5, 5);
        Point::MoveOn(15, 80);
        Point::MoveOn(50, 0);
    }
    else if (TypeSignal::Is(TypeSignal::_4))
    {
        Axis().Draw(Coord(x, y + 150), 170, 330, 00);

        Text("Ua").Write(x - 25, y + 10);

        VMeasuringLines(x + 40, x + 95, x + 105, y + 20, y + 80).Draw(); //-V525

        VMeasuringLines(x + 200, x + 210, x + 300, y + 20, y + 55).Draw();

        HMeasuringLines(x + 75, x + 115, y + 80, y + 110, y + 120).Draw();

        HMeasuringLines(x + 155, x + 255, y + 57, y + 110, y + 120).Draw();

        HMeasuringLines(x + 255, x + 275, y + 20, y + 110, y + 130).Draw();

        Point().Draw(x, y + 20, Color::WHITE);
        Point::MoveOn(40, 0);
        Point::MoveOn(35, 60);
        Point::MoveOn(40, 0);
        Point::MoveOn(40, -25);
        Point::MoveOn(100, 0);
        Point::MoveOn(20, -35);
        Point::MoveOn(40, 0);
    }
    else if (TypeSignal::Is(TypeSignal::_5a))
    {
        Axis().Draw(Coord(x, y + 150), 170, 330, 00);

        VMeasuringLines(x + 20, x + 220, x + 230, y, y + 130).Draw();

        HMeasuringLines(x + 30, x + 200, y + 80, y + 110, y + 130).Draw();

        Point().Draw(x, y + 130, Color::WHITE);
        Point::MoveOn(30, 0);
        Point::MoveOn(30, -120);
        Point::MoveOn(10, -10);
        Point::MoveOn(10, 0);
        Point::MoveOn(10, 10);
        Point::MoveOn(80, 100);
        Point::MoveOn(15, 10);
        Point::MoveOn(40, 10);
        Point::MoveOn(50, 0);
    }
    else if (TypeSignal::Is(TypeSignal::_5b))
    {
        Axis().Draw(Coord(x, y + 150), 170, 330, 00);

        VMeasuringLines(x + 20, x + 280, x + 290, y, y + 130).Draw();

        VMeasuringLines(x + 48, x + 240, x + 250, y + 60, y + 130).Draw();

        HMeasuringLines(x + 30, x + 200, y + 80, y + 110, y + 130).Draw();

        Point().Draw(x, y + 130, Color::WHITE);
        HLine(80).Draw(x + 48, y + 60);
        Point::MoveOn(30, 0);
        Point::MoveOn(30, -120);
        Point::MoveOn(10, -10);
        Point::MoveOn(10, 0);
        Point::MoveOn(10, 10);
        Point::MoveOn(80, 100);
        Point::MoveOn(15, 10);
        Point::MoveOn(40, 10);
        Point::MoveOn(50, 0);
    }

    Page::ForSignal(TypeSignal::Current())->DrawParameters();
}


void VoltageMode::Draw()
{
    static const pchar modes[Count] =
    {
        "12V",
        "24V"
    };

    Color::WHITE.SetAsCurrent();

    Text(modes[Current()]).Write(440, 250);
}
