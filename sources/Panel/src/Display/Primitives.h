// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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
        void Draw(int x, int y, Color color);
        void Draw(int x, int y);
    private:
        const int length;
    };


    class VLine
    {
    public:
        VLine(int l) : length(l) {};
        void Draw(int x, int y);
    private:
        const int length;
    };


    // ������������ ����� - ��� ����� 45 ��������
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
        void Draw(int x1, int y1, int x2, int y2);
    };
}
