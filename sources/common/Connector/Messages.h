// 2024/02/01 10:54:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Connector/Command.h"
#include "Menu/Value.h"


namespace Message
{
    struct MessageBase
    {
        MessageBase(Command::E c) : command(c) { }

        void Send();

    private:

        Command::E command;
    };


    struct Start1A : public MessageBase
    {
        Start1A() : MessageBase(Command::START_1) { }
    };


    struct Start2A : public MessageBase
    {
        Start2A(const Value &Us, const Value &t1) : MessageBase(Command::START_2A) { }
    };


    struct Start2B : public MessageBase
    {
        Start2B() : MessageBase(Command::START_2B) { }
    };


    struct Start3A : public MessageBase
    {
        Start3A() : MessageBase(Command::START_3A) { }
    };


    struct Start3B : public MessageBase
    {
        Start3B() : MessageBase(Command::START_3B) { }
    };

    struct Start4 : public MessageBase
    {
        Start4() : MessageBase(Command::START_4) { }
    };

    struct Start5A : public MessageBase
    {
        Start5A() : MessageBase(Command::START_5A) { }
    };

    struct Start5B : public MessageBase
    {
        Start5B() : MessageBase(Command::START_5B) { }
    };

    struct Stop : public MessageBase
    {
        Stop() : MessageBase(Command::STOP) { }
    };
}
