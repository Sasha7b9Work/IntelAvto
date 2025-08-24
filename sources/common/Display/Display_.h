// 2023/09/08 20:57:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Device/Value_.h"


class DataZone;
class ProgressBarTimeMeasureZone;


struct WarningMessage
{
    enum E
    {
        LittleTimeHeavyImpulse,
        PictureSaved,               // Изображение сохранено на флешку
        Count
    };
};


namespace Display
{
    extern Value value_in;
    extern Value value_out;

    // Число частей, на которые поделен дисплей для отрисовки
    const int NUM_PARTS = 2;

    extern uint8 buffer[272/Display::NUM_PARTS][480];

    extern int num_sends;

    const int PHYSICAL_WIDTH = 480;
    const int PHYSICAL_HEIGHT = 272;

    extern int width;
    extern int height;

    int Width();

    int Height();

    void Init();

    void Update();

    void BeginScene(int x = -1, int y = -1);

    void EndScene();

    void DrawWelcomeScreen();
    void DrawKeyboardFailScreen();

    // Номер верхней строки в текущей отрисовке.
    // Отрисовка происходит в два этапа - сначала верхняя часть (TopRow() == 0),
    // затем нижняя (TopRow() == Display/HEIGHT / 2)
    int TopRow();

    // Возвращает true, если прямоугльник находится в отрисовываемой в данный момент времени части экрана
    bool InDrawingPart(int y, int height);

    // Отрисовать очередную часть экрана
    void DrawPartScreen(int num, bool debugInfo);

    void SendToSCPI();

    // Здесь отрисовка результата измерения
    extern DataZone *zoneData;

    // Отрисовка времени отсчёта времени измерения
    extern ProgressBarTimeMeasureZone *zoneProgressBarTimeMeasure;

    void ShowWarningMessage(WarningMessage::E, uint time);

    void ShowFlashDriveMessage(bool);

    // Сохранить картинку на флешку
    void SaveToFlashDrive();

    // Структура для непосредсвенного рисования в дисплей
    namespace Sender
    {
        // Подготовить дисплей к непосредственному рисованию в аппаратуру
        void Prepare(int width, int height);

        // Послать в дисплей область, ограниченную (x0, y0) и ранее засланными (width, height)
        void SendToFSMC(int x0, int y0);

        // Восстановить к нормальному рисованию.
        void Restore();
    };

    // Эта функция только для win-версии
    void Draw(const uint *buffer);

    int Ymax();

    // В этом месте будем выводить постоянные параметры
    static const int yConstParameters = 250;
    static const int xConstParameters = 10;
};
