// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Console_.h"
#include "Display/Display_.h"
#include <cstring>
#include <cstdio>


namespace Console
{
#define S_DBG_NUM_STRINGS_IN_CONSOLE 1

    static char buffer[10][100];
    // true означает, что идЄт процесс вывода консоли и добавл€ть в неЄ новые строки нельз€ (это происходит, когда добавление идЄт из прерывани€)
    static bool inProcessDraw = false;
    // true, если происходит процесс добавлени€ строки
    static bool inProcessAddingString = false;
    //  оличество заполненных строк в консоли
    static int stringInConsole = 0;
    // «десь сохран€етс€ предыдущее значение максимального количества строк в консоли
    static int16 prevMaxStrinsInConsole = -1;

    void Init()
    {
        inProcessDraw = false;
        inProcessAddingString = false;
        stringInConsole = 0;
        prevMaxStrinsInConsole = -1;
    }

    static void DeleteFirstString();

    // ¬озвращает true, если консоль зан€та и с ней нельз€ производить работу
//    static bool IsBusy();
}


void Console::Draw()
{
/*
    if (prevMaxStrinsInConsole == -1)
    {
        prevMaxStrinsInConsole = S_DBG_NUM_STRINGS_IN_CONSOLE;
    }

    if (!IsShown() || stringInConsole == 0)
    {
        return;
    }

    inProcessDraw = true;

    int y = 5;

    for (int i = 0; i < stringInConsole; i++)
    {
        String string(buffer[i]);
        Primitives::Rect(string.Length() + 1, 15).Fill(0, y, Color::BLACK);
        Text(string.c_str()).Write(1, y, Color::_WHITE);
        y += 15;
    }

    inProcessDraw = false;
*/
}


void Console::DeleteFirstString()
{
    for (int16 i = 1; i < stringInConsole; i++)
    {
        std::strcpy(buffer[i - 1], buffer[i]);
    }
    stringInConsole--;
}


void Console::AddString(pchar string)
{
    inProcessAddingString = true;

    static int count = 0;
    if (stringInConsole == S_DBG_NUM_STRINGS_IN_CONSOLE)
    {
        DeleteFirstString();
    }
    std::sprintf(buffer[stringInConsole], "%d %s", count++, string);
    stringInConsole++;

    inProcessAddingString = false;

    Display::Update();
}


bool Console::IsShown()
{
    return true;
}


void Console::OnChanged_MaxStringsInConsole()
{
    // \todo «десь, видимо, не совсем корректное поведение в случае, когда реальных строк меньше, чем максимально допустимое их количество

    int delta = prevMaxStrinsInConsole - S_DBG_NUM_STRINGS_IN_CONSOLE;

    for (int i = 0; i < delta; i++)
    {
        DeleteFirstString();
    }

    prevMaxStrinsInConsole = S_DBG_NUM_STRINGS_IN_CONSOLE;


}


//bool Console::IsBusy()
//{
//    return inProcessDraw || inProcessAddingString;
//}
