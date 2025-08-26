// 2023/09/08 20:56:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Pages.h"
#include "Device/Device.h"
#include "Display/Display_.h"
#include "Display/Font/Font.h"
#include "Display/Text_.h"
#include "Display/Console_.h"
#include "Display/Pictures/Picture.h"
#include "FlashDrive/FlashDrive.h"
#include "LAN/LAN.h"
#include "Hardware/Timer.h"
#include "Device/IT6523.h"
#include "FlashDrive/FlashDrive.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


using namespace Primitives;


namespace Display
{
    Value value_in(0, TypeValue::Voltage);
    Value value_out(0, TypeValue::Voltage);

    int num_sends = 0;

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

    static bool show_flash_drive_message = false;
    static uint time_start_show_flash_drive = 0;

    static void WriteFlashDriveMessage();

    static void DrawFlashDrive(int x, int y);
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
    for (int i = 0; i < NUM_PARTS; i++)
    {
        Device::TasksUpdate();
        DrawPartScreen(i, true);
    }
}


void Display::SaveToFlashDrive()
{
    show_flash_drive_message = false;

#pragma pack(push)

#pragma pack(1)
    typedef struct
    {
        char    type0;      // 0
        char    type1;      // 1
        uint    size;       // 2
        uint16  res1;       // 6
        uint16  res2;       // 8
        uint    offBits;    // 10
    } BITMAPFILEHEADER;
    // 14

    typedef struct
    {
        uint    size;           // 14
        int     width;          // 18
        int     height;         // 22
        uint16  planes;         // 26
        uint16  bitCount;       // 28
        uint    compression;    // 30
        uint    sizeImage;      // 34
        int     xPelsPerMeter;  // 38
        int     yPelsPerMeter;  // 42
        uint    clrUsed;        // 46
        uint    clrImportant;   // 50
        //uint    notUsed[15];
    } BITMAPINFOHEADER;
    // 54
#pragma pack(4)

    BITMAPFILEHEADER bmFH =
    {
        0x42,
        0x4d,
        14 + 40 + 1024 + 320 * 240 / 2,
        0,
        0,
        14 + 40 + 1024
    };

    StructForWrite structForWrite;
    char fileName[255];

    std::sprintf(fileName, "%09d.bmp", TIME_MS);

    FDrive::OpenNewFileForWrite(fileName, &structForWrite);

    FDrive::WriteToFile((uint8 *)(&bmFH), 14, &structForWrite);

    BITMAPINFOHEADER bmIH =
    {
        40, // size;
        Display::PHYSICAL_WIDTH,// width;
        Display::PHYSICAL_HEIGHT,// height;
        1,  // planes;
        8,  // bitCount;
        0,  // compression;
        0,  // sizeImage;
        0,  // xPelsPerMeter;
        0,  // yPelsPerMeter;
        0,  // clrUsed;
        0   // clrImportant;
    };

    FDrive::WriteToFile((uint8 *)(&bmIH), 40, &structForWrite);

    typedef struct tagRGBQUAD
    {
        uint8    blue;
        uint8    green;
        uint8    red;
        uint8    rgbReserved;
    } RGBQUAD;

    RGBQUAD colorStruct;

    uint8 wr_buffer[480];

    for (int i = 0; i < 16; i++)
    {
        uint color = Color((uint8)i).Value();
        colorStruct.blue = BLUE_FROM_COLOR(color);
        colorStruct.green = GREEN_FROM_COLOR(color);
        colorStruct.red = RED_FROM_COLOR(color);
        colorStruct.rgbReserved = 0;
        ((RGBQUAD *)(wr_buffer))[i] = colorStruct;
    }

    for (int i = 0; i < 4; i++)
    {
        FDrive::WriteToFile(wr_buffer, 256, &structForWrite);
    }

    for (int part = NUM_PARTS - 1; part >= 0; part--)
    {
        DrawPartScreen(part, false);

        for (int y = Display::PHYSICAL_HEIGHT / 2 - 1; y >= 0; y--)
        {
            FDrive::WriteToFile(&buffer[y][0], Display::PHYSICAL_WIDTH, &structForWrite);
        }
    }

    FDrive::CloseFile(&structForWrite);

    Display::ShowWarningMessage(WarningMessage::PictureSaved, 1000);

#pragma pack(pop)
}


void Display::DrawPartScreen(int num, bool)
{
    SetTopRow(num);

    if (num == 0)
    {
        timeStart = TIME_MS;
    }

    Device::TasksUpdate();

    Display::BeginScene();

    Device::TasksUpdate();

    DrawScreen();

    Device::TasksUpdate();

    if (num == 0)
    {
        Console::Draw();
    }

    DrawFlashDrive(PHYSICAL_WIDTH - 50, PHYSICAL_HEIGHT - 30);

    if (Device::IsRunning())
    {
        Font::SetSize(3);

        if (ColorTimer::IsMain())
        {
            int x = 130;
            int y = 95;
            int d = 5;

            Rect(310 + 2 * d, 45 + 2 * d).Fill(x - d, y - d, Color::BACK);

            Text("ТЕСТИРОВАНИЕ").Write(x, y, Color::RED);
        }

        Font::SetSize(1);
    }

    if (num_sends > 1)
    {
        Text("%d", num_sends).Write(420, 240, Color::WHITE);
    }

    Device::TasksUpdate();

    if (!TypePicture::IsExtern() && PageSettings::show_debug_voltage)
    {
        Text("%.2f", (double)value_in.ToUnits()).Write(400, 5, Color::WHITE);
        Text("%.2f", (double)value_out.ToUnits()).Write(400, 30);
    }

    WriteFlashDriveMessage();

    Display::EndScene();
    Device::TasksUpdate();

    if (num == Display::NUM_PARTS)
    {
        timeFrame = TIME_MS - timeStart;
    }
}


void Display::DrawScreen()
{
    Menu::Draw();

    if (Menu::OpenedPageIsSignal())
    {
        Text("Тип сигнала %s : %s",
            TypePicture::ToString(),
            TypePicture::Name()).Write(230, 6, Color::WHITE);

        DrawSignal();
    }
    else if (Menu::OpenedPage() == PageInfo::self ||
        Menu::OpenedPage()->ConsistOpenedItems() ||
        Menu::OpenedPage() == PageCalibration::self)
    {

    }
    else
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
        if (IT6523::TimeLeftToHeavyImpulse() == 0 && warn == WarningMessage::LittleTimeHeavyImpulse)
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

            if (warn == WarningMessage::LittleTimeHeavyImpulse)
            {
                Rect(w, h).FillRounded(x, y, 2, Color::BACK, Color::WHITE);

                if ((((TIME_MS - time_warn) / 500) % 2) == 0)
                {
                    Text("Импульс можно включить через %d секунд", IT6523::TimeLeftToHeavyImpulse()).
                        Write(x + d, y + d, Color::WHITE);
                }
            }
            else if (warn == WarningMessage::PictureSaved)
            {
                w = 220;
                x = 170;

                Rect(w, h).FillRounded(x, y, 2, Color::BACK, Color::WHITE);

                Text("Изображение сохранено").Write(x + d, y + d, Color::WHITE);
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
    TypePicture::E type = TypePicture::Current();

    if (gset.type_picture)
    {
        if (type == TypePicture::_1 ||
            type == TypePicture::_2a ||
            type == TypePicture::_3a ||
            type == TypePicture::_3b)
        {
            type = TypePicture::Scheme1;
        }
        else
        {
            type = TypePicture::Scheme2;
        }
    }

    Picture::DrawPicure(150, 50, type);

    Page::ForCurrentSignal()->DrawParameters();
}


void Display::ShowWarningMessage(WarningMessage::E _warn, uint time)
{
    warn = _warn;

    time_warn = TIME_MS;

    Timer::SetDefferedOnceTask(TimerTask::DisplayWarningMessage, time, DisableWarningMessage);
}


void Display::DisableWarningMessage()
{
    warn = WarningMessage::Count;
}


void Display::ShowFlashDriveMessage(bool show)
{
    show_flash_drive_message = show;

    if (show)
    {
        if (time_start_show_flash_drive == 0)
        {
            time_start_show_flash_drive = TIME_MS;
        }
    }
    else
    {
        time_start_show_flash_drive = 0;
    }
}


void Display::WriteFlashDriveMessage()
{
    if (show_flash_drive_message)
    {
        const int x = 75;
        const int y = 100;

        Rect(350, 70).FillRounded(x, y, 1, Color::BACK, Color::WHITE);

        const int d = 15;

        if (ColorTimer::IsMain())
        {
            Text("Обнаружено запоминающее устройство.").Write(x + d, y + d, Color::WHITE);

            uint dT = TIME_MS - time_start_show_flash_drive;

            uint time = 0;

            if (dT < 30000)
            {
                time = (30000 - dT) / 1000;
            }
            else
            {
                time_start_show_flash_drive = TIME_MS;
            }

            Text("Подключение... Осталось %d сек...", time).Write(x + d, y + d + 30);
        }
    }
}


void Display::DrawFlashDrive(int x, int y)
{
    if (!FDrive::IsConnected())
    {
        return;
    }

    const int w = 50;
    const int h = 30;

    Rect(w, h).Draw(x, y, Color::WHITE);

    HLine(w - 10).Draw(x + 5, y + h / 2);

    Rect(6, 6).FillRounded(x + w - 10, y + h / 2 - 3, 2, Color::WHITE, Color::WHITE);

    const int d = 5;

    for (int i = 1; i < d; i++)
    {
        Line().Draw(x + 5, y + h / 2, x + 5 + 7, y + h / 2 - d + i);
        Line().Draw(x + 5, y + h / 2, x + 5 + 7, y + h / 2 + d - i);
    }

    Rect(5, 5).Fill(x + 15, y + 4);

    HLine(10).Draw(x + 15, y + 6);

    Line().Draw(x + 25, y + 6, x + 30, y + h / 2);

    Rect(4, 4).FillRounded(x + 18, y + h - 9, 1, Color::WHITE, Color::WHITE);
    Rect(3, 3).FillRounded(x + 18, y + h - 8, 1, Color::WHITE, Color::WHITE);

    HLine(10).Draw(x + 23, y + h - 7);

    Line().Draw(x + 33, y + h - 7, x + 38, y + h - 7 - 8);
}
