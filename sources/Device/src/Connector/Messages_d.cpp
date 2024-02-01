// 2024/02/01 23:25:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Connector/Messages.h"
#include "Generator/Generator.h"


void Message::Start2A::Execute()
{
    Value Us = PopValue();

    Value t1 = PopValue();

    Generator::Start2A(Us, t1);
}
