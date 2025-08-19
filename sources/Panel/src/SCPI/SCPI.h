// 2025/02/11 09:41:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer_.h"
#include "Utils/String_.h"


namespace SCPI
{
    struct Command;

    class InBuffer : public Buffer2048
    {
    public:
        InBuffer() = default;
        void Update();
    private:
        Command *ParseCommand(pchar);
        String<> FirstWord(pchar);
        Command *ExtractCommand();
    };

    void AddNewData(uint8 *, uint);

    void Update();
}
