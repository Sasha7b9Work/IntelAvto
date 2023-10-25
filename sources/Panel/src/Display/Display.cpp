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
#include "Display/RedrawingZone.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


using namespace Primitives;


// Нарисовать подсказку
static void DrawHint(int x, int y);

static void DrawInfo();

static void SetTopRow(int i);


static bool needRedraw = true;      // Если true, требуется перерисовка дисплея
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


struct Coord
{
    int x;
    int y;
};


static int yString = 110;

static Coord coordMemory = { 15, yString };
static Coord coordTest = { 40, yString };
static Coord coordExtGenerator = { 95, yString };
static Coord coordLaunch = { 130, yString };


static const int MAX_OBJECTS = 10;
static RedrawingZone *objects[MAX_OBJECTS];
static int numObjects = 0;

static DataZone sDataZone;
DataZone *Display::zoneData = &sDataZone;

static ProgressBarTimeMeasureZone sProgressBarTimeMeasureZone;
ProgressBarTimeMeasureZone *Display::zoneProgressBarTimeMeasure = &sProgressBarTimeMeasureZone;


static void AddObject(RedrawingZone *object)
{
    objects[numObjects++] = object;
}


void Display::Init()
{
    InitHardware();

    Font::Set(TypeFont::GOSTAU16BOLD);

    Font::SetSpacing(2);

    AddObject(zoneData);
    AddObject(zoneProgressBarTimeMeasure);
}


static void DrawValue(pString string, int x, int y)
{
    Rectangle(Font::GetLengthText(string) + 5, 20).FillRounded(x - 3, y - 3, 2, Color::BACK, Color::FILL);

    Text(string).Write(x, y, Color::FILL);
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


void Display::Refresh()
{
    needRedraw = true;

    for (int i = 0; i < numObjects; i++)
    {
        objects[i]->Refresh();
    }
}


void Display::Update()
{
#ifdef GUI

    BeginScene();
    DrawScreen();
    Console::Draw();
    EndScene();

#else 

    static uint currentFramesInSec = 0;         // Столько кадров отрисовано за текущую секунду
    static uint currentTimePaintInSec = 0;      // Столько времени потрачено на отрисовку за текущую секунду
    static char prevHint = 0;

    zoneProgressBarTimeMeasure->Refresh();      // Прогресс-бар будем перерисовывать каждый кадр

    if (Hint::Text()[0] != prevHint)            // Если изменилась подсказка - перерисовываем экран
    {
        prevHint = Hint::Text()[0];

        Display::Refresh();
    }

    if (timeAutoHint != 0U && (TIME_MS - timeAutoHint) > 10000)
    {
        timeAutoHint = 0U;
        Display::Refresh();
    }

    if (needRedraw)
    {
        timeStart = TIME_MS;

        for (int i = 0; i < NUM_PARTS; i++)
        {
            DrawPartScreen(i, true);
        }

        timeFrame = TIME_MS - timeStart;
        currentFramesInSec++;
        currentTimePaintInSec += timeFrame;
    }

    for (int i = 0; i < numObjects; i++)
    {
        objects[i]->Update(RedrawingZone::ModeDraw::ToHardware);
    }

    needRedraw = false;

    if (TIME_MS >= beginSecond + 1000)
    {
        fps = currentFramesInSec;
        currentFramesInSec = 0;
        beginSecond = TIME_MS;
        timePaint = currentTimePaintInSec;
        currentTimePaintInSec = 0;
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
    if (PageIndication::calibrationMode.IsEnabled())
    {
        Text(LANG_IS_RU ? "---Режим Калибровка---" : "---Calibration mode---").Write(140, 10, Color::FILL);
        Text(LANG_IS_RU ? "Нажмите ЭНК. для сохранения" : "Click ENC. to preserve").Write(125, 40);
        Text(LANG_IS_RU ? "Нажмите любую кнопку для выхода" : "Press any button to exit").Write(105, 70);
    }
    else
    {
        DrawHint(10, Display::PHYSICAL_HEIGHT - Item::HEIGHT - 30);

        DrawInfo();

        for (int i = 0; i < numObjects; i++)
        {
            objects[i]->Update(RedrawingZone::ModeDraw::ToBuffer);
        }

        Menu::Draw();
    }
}


static void DrawHint(int x, int y)
{
    static bool autoFlag = false;

    if ((TIME_MS < timeAutoHint + 10000) && timeAutoHint != 0 && autoFlag)
    {
        Rectangle(360, 30).FillRounded(x, y, 2, Color::BACK, Color::BACK);
    }
    else
    {
    }
}


static void DrawInfo()
{
    if (PageIndication::memoryMode == MemoryMode::On)
    {
    }

    if (PageIndication::refGenerator == RefGenerator::External)
    {
        DrawValue(LANG_IS_RU ? "ОГ" : "Ref", coordExtGenerator.x, coordExtGenerator.y);
    }

    if (PageIndication::launchSource.IsExternal())
    {
        DrawValue(LANG_IS_RU ? "Зап:внешн" : "Launch:ext", coordLaunch.x, coordLaunch.y);
    }
    else if (PageIndication::launchSource.IsOneTime())
    {
        DrawValue(LANG_IS_RU ? "Зап:однокр" : "Launch:single", coordLaunch.x, coordLaunch.y);
    }

    if (PageIndication::launchSource == LaunchSource::OneTime)
    {
        if (PageIndication::OnceLaunch() == true)
        {
            if (second == 0)
            {
                second = (int)TIME_MS;
            }
            Text(LANG_IS_RU ? "ПУСК" : "START").Write(430, 110);
            if ((second + 1000) < (int)TIME_MS)
            {
                second = 0;
                PageIndication::OnceLaunchSwitchFalse();
            }
        }
        else
        {
            Text(" ").Write(430, 80);
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

    Refresh();
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
