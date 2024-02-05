// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Interface_p.h"
#include "Connector/Transceiver.h"
//#include "structs.h"
#include "Display/Console.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"
#include "Connector/Handlers_p.h"
#include <cstdlib>


static List<Task> tasks;


namespace PInterface
{
    // ќбрабатывает answer, если запрос на него есть в очереди заданий. ¬озвращает true, если это так
    bool ProcessTask(BaseMessage *answer);

    // ќбрабатывает очередь заданий, засыла€ сообщени€ тех из них, которые необходимо заслать
    void SendTasks();

    // ќбработать ответ на задание
    void RunAnswer(ListElement<Task> *element, BaseMessage *answer);
}


void PInterface::AddMessageForTransmit(BaseMessage *message)
{
    Transceiver::Transmit(message);
}


void PInterface::Update()
{
    static uint time = 0;

    if (_TIME_MS - time < 100)
    {
        return;
    }

    time = _TIME_MS;

    Message::RequestData message;

    Transceiver::Transmit(&message);
    
    if (Transceiver::Receive(&message))
    {
        if (ProcessTask(&message) ||            // ќбрабатываем сообщение, если запрос на него есть в очереди заданий
            PHandlers::Processing(&message))    // или просто обрабатываем в обратном случае
        {
            time = 0;
            Update();
        }
    }

    SendTasks();
}


void PInterface::AddTask(Task *task)
{
    if(!tasks.IsMember(task))                   // ≈сли задани€ ещЄ нет в очереди
    {
        task->TransmitMessage();                // “о посылаем его сообщение
        tasks.Append(task);                     // и добавл€ем в очередь сообщений дл€ повторной отправки
    }
}


void Task::TransmitMessage()
{
    message->Transmit();    // ѕосылаем сообщение
    timeLast = _TIME_MS;     // запоминаем врем€ посылки
}


void PInterface::SendTasks()
{
    ListElement<Task> *element = tasks.First();

    while (element)
    {
        Task *task = element->Get();

        if (!task->PassedLittleTimeAfterSend())
        {
            task->TransmitMessage();
        }

        element = element->Next();
    }
}


bool Task::PassedLittleTimeAfterSend()
{
    return (_TIME_MS - timeLast) < 1000;
}


bool PInterface::ProcessTask(BaseMessage *answer)
{
    ListElement<Task> *element = tasks.First();

    Task taskAnswer(answer);

    while (element)
    {
        Task *task = element->Get();

        if (task->Equals(task, &taskAnswer))
        {
            RunAnswer(element, answer);
            return true;
        }

        element = element->Next();
    }


    return false;
}


void PInterface::RunAnswer(ListElement<Task> *element, BaseMessage *answer)
{
    element->Get()->funcProcess(answer);
    tasks.Remove(element->Get());
    delete element;
}


bool Task::Equals(Task *task1, Task *task2)
{
    return funcEqual(task1, task2);
}


Task::Task(BaseMessage *msg) : funcProcess(nullptr), timeLast(0), funcEqual(nullptr)
{
    message = msg->Clone();
}


Task::Task(BaseMessage *msg, bool(*process)(BaseMessage *), bool(*equal)(Task *, Task *)) :funcProcess(process), timeLast(0), funcEqual(equal)
{
    message = msg->Clone();
}


Task::~Task()
{
    delete message;
}


BaseMessage *Task::GetMessage()
{
    message->ResetPointer();
    return message;
}


void BaseMessage::Transmit()
{
    PInterface::AddMessageForTransmit(this);
}
