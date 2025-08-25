// 2025/02/11 09:41:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Buffer_.h"
#include "Utils/String_.h"

/*
+   *IDN?
+   :SIGNAL:SET <1, 2a, 2b, 3a, 3b, 4, 5a, 5b>
+   :SIGNAL:GET?
+   :MODE:SET <12V, 24>
+   :MODE:GET?
                         1 2a 2b 3a 3b 4 5a 5b
    :PARAM:Us:SET        + +     +  +  + +  +
    :PARAM:td:SET             +          +  +
    :PARAM:Ri:SET                        +  +
    :PARAM:N:SET         + +  +  +  +  + +  +
    :PARAM:Period:SET    + +
    :PARAM:Imax:SET      + +     +  +
    :PARAM:Ua:SET                      +
    :PARAM:t7:SET                      +
    :PARAM:t9:SET                      +
*/


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

    void AppendNewData(uint8 *, uint);

    void Update();
}
