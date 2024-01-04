// 2023/12/25 16:59:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Signals/PageSignals.h"
#include "Menu/MenuItemsDef.h"


//static void OnPress_TypeSignal();
//
//
//// Выбор типа сигнала
//DEF_SWITCH_8(sTypeSignal,
//    "Сигнал", "Signal",
//    "1", "2a", "2b", "3a", "3b", "4", "5a", "5b",
//    "1", "2a", "2b", "3a", "3b", "4", "5a", "5b",
//    PageSignals::typeSignal, OnPress_TypeSignal
//);


Item *items[7] =
{
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};


void OnPress_TypeSignal()
{
    if (TypeSignal::Is(TypeSignal::_2b))
    {
        PageSignals::_2b::SetParameters(&items[1]);
    }
    else if (TypeSignal::Is(TypeSignal::_3a))
    {
        PageSignals::_3a::SetParameters(&items[1]);
    }
    else if (TypeSignal::Is(TypeSignal::_3b))
    {
        PageSignals::_3b::SetParameters(&items[1]);
    }
    else if (TypeSignal::Is(TypeSignal::_4))
    {
        PageSignals::_4::SetParameters(&items[1]);
    }
    else if (TypeSignal::Is(TypeSignal::_5a))
    {
        PageSignals::_5a::SetParameters(&items[1]);
    }
    else if (TypeSignal::Is(TypeSignal::_5b))
    {
        PageSignals::_5b::SetParameters(&items[1]);
    }
}


static Page pageIndication(items, nullptr, nullptr);


pchar TypeSignal::Name(E v)
{
    static const pchar names[Count] =
    {
        "1",
        "2a",
        "SAEJ1113-11",
        "3a",
        "3b",
        "DIN40839",
        "16750-1",
        "16750-1"
    };

    return names[v];
}


namespace PageSignals
{
    namespace _2a
    {
        static Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_2a].values[0]);

        static Parameter param_t1("t1", "t1", gset.signals[TypeSignal::_2a].values[1]);

        void SetParameters(Item **first)
        {
            *first++ = &param_Us;
            *first++ = &param_t1;
            *first++ = nullptr;
        }
    }

    namespace _2b
    {
        static Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_2b].values[0]);

        static Parameter param_td("td", "td", gset.signals[TypeSignal::_2b].values[1]);

        void SetParameters(Item **first)
        {
            *first++ = &param_Us;
            *first++ = &param_td;
            *first++ = nullptr;
        }
    }

    namespace _3a
    {
        static Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_3a].values[0]);

        void SetParameters(Item **first)
        {
            *first++ = &param_Us;
            *first++ = nullptr;
        }
    }

    namespace _3b
    {
        static Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_3b].values[0]);

        void SetParameters(Item **first)
        {
            *first++ = &param_Us;
            *first++ = nullptr;
        }
    }

    namespace _4
    {
        static Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_4].values[0]);

        static Parameter param_Ua("Ua", "Ua", gset.signals[TypeSignal::_4].values[1]);

        static Parameter param_t7("t7", "t7", gset.signals[TypeSignal::_4].values[2]);

        static Parameter param_t9("t9", "t9", gset.signals[TypeSignal::_4].values[3]);

        static Parameter param_t11("t11", "t11", gset.signals[TypeSignal::_4].values[4]);

        void SetParameters(Item **first)
        {
            *first++ = &param_Us;
            *first++ = &param_Ua;
            *first++ = &param_t7;
            *first++ = &param_t9;
            *first++ = &param_t11;
            *first++ = nullptr;
        }
    }

    namespace _5a
    {
        static Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_5a].values[0]);

        static Parameter param_td("td", "td", gset.signals[TypeSignal::_5a].values[1]);

        void SetParameters(Item **first)
        {
            *first++ = &param_Us;
            *first++ = &param_td;
            *first++ = nullptr;
        }
    }

    namespace _5b
    {
        static Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_5b].values[0]);

        static Parameter param_Uss("Us*", "Us*", gset.signals[TypeSignal::_5b].values[1]);

        static Parameter param_td("td", "td", gset.signals[TypeSignal::_5b].values[2]);

        void SetParameters(Item **first)
        {
            *first++ = &param_Us;
            *first++ = &param_Uss;
            *first++ = &param_td;
            *first++ = nullptr;
        }
    }
}
