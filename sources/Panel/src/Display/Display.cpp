// 2023/09/08 20:56:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Log.h"
#include "Settings.h"
#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/Primitives.h"
#include "Display/Text.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageIndication.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


using namespace Primitives;


// Нарисовать подсказку
static void DrawHint(int x, int y);

static void DrawInfo();

static void SetTopRow(int i);


static uint timeAutoHint = 0;
static int second = 0;
static int topRow = 0;


static uint timeStart = 0;
static uint timeFrame = 0;
static uint fps = 0;                // Столько кадров отрисовано за последнюю секунду
static uint beginSecond = 0;        // В это время началась последняя секунда
static uint timePaint = 0;          // Время отрисовки за секунду


static int topDraw = 0;             // Верхний у отрисовываемой части экрана
static int bottomhDraw = 0;         // Нижний у отрисовываемой части экрана

int Display::width = Display::PHYSICAL_WIDTH;
int Display::height = Display::PHYSICAL_HEIGHT;


bool Display::sendToSCPI = false;
bool Display::drawingScene = false;


static int yString = 110;

static Coord coordMemory = { 15, yString };
static Coord coordTest = { 40, yString };
static Coord coordExtGenerator = { 95, yString };
static Coord coordLaunch = { 130, yString };

static void DrawSignal();


void Display::Init()
{
    InitHardware();

    Font::Set(TypeFont::GOSTAU16BOLD);

    Font::SetSpacing(2);
}


void DrawRectangle1(int x, int y)
{
    static uint time_start = TIME_MS;

    const int w = Display::PHYSICAL_WIDTH - x * 2;
    const int h = Display::PHYSICAL_HEIGHT - y * 2;
    const int x_start = x;
    const int x_end = x + w;
    const int y_start = y;
    const int y_end = y + h;

    int cell = 10;

    Rectangle(w, h).Draw(x, y);
    Rectangle(w - cell * 2, h - cell * 2).Draw(x + cell, y + cell);

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

        Rectangle(w, h).Draw(x, y);

        x -= cell;
        y -= cell;
    }

    Rectangle(Display::PHYSICAL_WIDTH, Display::PHYSICAL_HEIGHT).Draw(0, 0);
}


void DrawRectangle3(int, int)
{
    Rectangle(Display::PHYSICAL_WIDTH, Display::PHYSICAL_HEIGHT).Draw(0, 0);

    static uint time_start = TIME_MS;

    int cell = 20;

    Rectangle(Display::PHYSICAL_WIDTH - cell * 2, Display::PHYSICAL_HEIGHT - cell * 2).Draw(cell, cell);

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
    Rectangle(Display::PHYSICAL_WIDTH, Display::PHYSICAL_HEIGHT).Draw(0, 0);

    static uint time_start = TIME_MS;

    int cell = 18;

    Rectangle(Display::PHYSICAL_WIDTH - cell * 2, Display::PHYSICAL_HEIGHT - cell * 2).Draw(cell, cell);

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

static void SetTopRow(int i)
{
    topRow = i * (Display::PHYSICAL_HEIGHT / Display::NUM_PARTS);
    topDraw = topRow;
    bottomhDraw = topDraw + Display::PHYSICAL_HEIGHT / Display::NUM_PARTS;
}


void Display::Update()
{
#ifdef WIN32
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

    Display::EndScene();

    if (num == Display::NUM_PARTS)
    {
        timeFrame = TIME_MS - timeStart;
    }
}


void Display::DrawScreen()
{
    Menu::Draw();

    Text(String(LANG_IS_RU ? "Тип сигнала : %s" : "Type signal : %s", PageIndication::typeSignal.ToString().c_str())).Write(300, 6, Color::WHITE);

    DrawSignal();
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

bool Display::InDrawingPart(int y, int height)
{
    if (!drawingScene)
    {
        return true;
    }

    int yBottom = y + height;

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


void DrawSignal()
{
    int x = 130;
    int y = 70;
    Coord coord = { x, y };

    Color::GRAY.SetAsCurrent();

    if (PageIndication::typeSignal == TypeSignal::_1)
    {
        Axis().Draw({ x, y + 25 }, 70, 330, 150);

        Text("Ua").Write(x - 25, y - 10);

        HMeasuringLines(x + 70, x + 290, y - 30, y - 15, y + 25, "t1").Draw();

        VMeasuringLines(x + 70, x + 200, x + 210, y + 25, y + 175, "Us").Draw();

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
    else if (PageIndication::typeSignal == TypeSignal::_2a)
    {
        Axis().Draw({ x, y + 170 }, 210, 330, 10);

        Text("Ua").Write(x - 25, y + 140);

        HMeasuringLines(x + 50, x + 245, y - 30, y - 15, y + 151, "t1").Draw();

        VMeasuringLines(x + 70, x + 180, x + 200, y, y + 150, "Us").Draw();

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
    else if (PageIndication::typeSignal == TypeSignal::_2b)
    {
        Axis().Draw({ x, y + 170 }, 210, 330, 10);

        Text("Ua").Write(x - 25, y + 10);

        HMeasuringLines(x + 70, x + 270, y - 5, y + 10, y + 170, "td").Draw();

        VMeasuringLines(x + 90, x + 290, x + 310, y + 45, y + 170, "Us").Draw();

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
}
