// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/Application.h"
#include "Menu/Menu.h"
#include "Hardware/Keyboard/Keyboard_.h"


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

    static void AddAction(Control control, Action::E action)
    {
        control.action = action;
        actions[numActions++] = control;
    }
}


bool Keyboard::Init()
{
    return true;
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


pchar Key::Name(E value)
{
    static pchar names[Key::Count] =
    {
        "None",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "0",
        "-",
        ".",
        "START",
        "<-",
        "->",
        "ESC",
        "OK",
        "STOP",
        "",
        "",
        ""
    };

    return names[value];
}


void Keyboard::Lock()
{
    Menu::Input::SetFuncUpdate(Menu::Input::FuncEmptyUpdate);
}


void Keyboard::Unlock()
{
    Menu::Input::SetFuncUpdate(Menu::Input::FuncUpdate);
}
