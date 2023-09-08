// 2022/01/13 13:50:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Page;


struct PageColors
{
    static Page *self;
};


struct PageInfo
{
    static Page *self;
};


struct PageService
{
    static Page *self;
    static void UseReset();
};


struct PageStatistics
{
    static Page *self;
    static void Clear();
};


struct PageTuneColors
{
    static Page *self;
    static void Init();
};
