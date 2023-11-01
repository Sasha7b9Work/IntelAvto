// 2023/09/08 21:15:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


namespace Primitives
{
    class Rectangle
    {
    public:
        Rectangle(int _width, int _height) : width(_width), height(_height) {}
        void Draw(int x, int y);
        void Draw(int x, int y, Color color);
        void DrawRounded(int x, int y, int round, Color color);
        void Fill(int x, int y);
        void Fill(int x, int y, Color color);
        void FillRounded(int x, int y, int round, Color colorFill, Color colorBound);
    private:
        int width;
        int height;
    };


    class Point
    {
    public:
        Point() {};
        void Draw(int x, int y, Color color);
        void Draw(int x, int y);
    };


    class HLine
    {
    public:
        HLine(int _length) : length(_length) {};
        int Draw(int x, int y, Color color);
        int Draw(const Coord &, Color color);
        int Draw(const Coord &);
        int Draw(int x, int y);
    private:
        const int length;
    };


    class VLine
    {
    public:
        VLine(int l) : length(l) {};
        int Draw(int x, int y, Color color);
        int Draw(int x, int y);
        int Draw(const Coord &);
    private:
        const int length;
    };


    // Диагональная линия - под углом 45 градусов
    class DLine
    {
    public:
        DLine(int _length, int _dx, int _dy) : length(_length), dx(_dx), dy(_dy) {};
        void Draw(int x, int y);
    private:
        const int length;
        const int dx;
        const int dy;
    };

    class Line
    {
    public:
        Coord Draw(int x1, int y1, int x2, int y2);
        Coord Draw(const Coord &, int x2, int y2);
    };

    class ArrowRight
    {
    public:
        static const int WIDTH = 15;
        void Draw(int x, int y);
    };

    class ArrowLeft
    {
    public:
        static const int WIDTH = 15;
        void Draw(int x, int y);
    };

    class ArrowUp
    {
    public:
        void Draw(int x, int y);
    };

    class HMeasuringLines
    {
//      x0                    x1  y0
//       |/       tex        \|   
//       +--------------------+   y1
//       |\                  /|
//       |                    |
//       |                    |   y2

    public:

        HMeasuringLines(int _x0, int _x1, int _y0, int _y1, int _y2, pchar _text) :
            m_x0(_x0), m_x1(_x1), m_y0(_y0), m_y1(_y1), m_y2(_y2), text(_text) { }

        void Draw(const Coord &delta = { 0, 0 });

    private:
        const int m_x0;
        const int m_x1;
        const int m_y0;
        const int m_y1;
        const int m_y2;
        pchar text;
    };

    class ExHMeasuringLines
    {
//
//
//
//
//
//
//
    };
}
