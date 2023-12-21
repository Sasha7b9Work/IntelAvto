// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Keyboard/Keyboard.h"
#include "Application_win.h"


namespace Keyboard
{
    // Очередь сообщений - здесь все события органов управления
#define MAX_ACTIONS 100
    static Control actions[MAX_ACTIONS];
    // Количество уже имеющихся сообщений
    static int numActions = 0;

    static bool needStartTimerLong = false;

    static bool needStopTimerLong = false;

    // Здесь имя нажатой кнопки
    static Key::E pressedKey = Key::None;

    static void AddAction(Control control, Action::E action);
}


bool Keyboard::Init()
{
    return true;
}


void Keyboard::AddAction(Control control, Action::E action)
{
    if (action != Action::Press)
    {
        return;
    }

    control.action = action;
    actions[numActions++] = control;
}


void Frame::OnDown(wxCommandEvent &event)
{
    Key::E key = (Key::E)event.GetId();

    //std::cout << "down " << Control(key).Name() << std::endl;
    event.Skip();

    Keyboard::AddAction(key, Action::Press);

    Keyboard::needStartTimerLong = true;

    Keyboard::pressedKey = key;
}


void Frame::OnUp(wxCommandEvent &event)
{
    Key::E key = (Key::E)event.GetId();

    //std::cout << "up   " << Control(key).Name() << std::endl;
    event.Skip();

    Keyboard::AddAction(key, Action::Release);

    Keyboard::needStopTimerLong = true;

    Keyboard::pressedKey = Key::None;
}


bool Keyboard::Empty()
{
    return numActions == 0;
}


Control Keyboard::NextControl()
{
    if (Empty())
    {
        return Control();
    }

    Control result = actions[0];

    for (int i = 1; i < numActions; i++)
    {
        actions[i - 1] = actions[i];
    }

    --numActions;

    return result;
}

String Key::Name(E value)
{
    static const char *names[Key::Count] =
    {
        /*  1 */   "Ручка нажать",
        /*  2 */   "РЕЖИМ",
        /*  3 */   "ИНДИКАЦИЯ",
        /*  4 */   "<-",
        /*  5 */   "->",
        /*  6 */   "КАНАЛЫ",
        /*  7 */   "ENTER",
        /*  8 */   "СЕРВИС",
        /*  9 */   "Ручка лево",
        /* 10 */   "Ручка право",
        /* 11 */   "ТЕСТ",
        /* 12 */   "АВТО",
        /* 13 */   "NULL"
    };

    return String(names[value]);
}

