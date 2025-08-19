// 2022/11/23 11:21:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer_.h"
#include "Utils/String_.h"


namespace SCPI
{
    struct Command
    {
        virtual bool Execute() = 0;
        virtual ~Command() { }
    };


    struct CommandNull : public Command
    {
        virtual bool Execute() override { return false; }
    };


    struct CommandWithParameters : public Command
    {
        CommandWithParameters(pchar _params) { params.SetFormat(_params); }
        virtual bool Execute() override;
    protected:
        String<> params;
    };


    struct CommandIDN : public Command
    {
        virtual bool Execute() override;
    };


    struct CommandSignalGet : public Command
    {
        virtual bool Execute() override;
    };
}
