// 2023/12/25 17:20:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Signals/PageSignals.h"
#include "Settings/Settings.h"


namespace PageSignals
{
    namespace _1
    {
        static Parameter param_t1("t1", "t1", gset.signals[TypeSignal::_1].values[0]);

        static Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_1].values[1]);

        void SetParameters(Item **first)
        {
            *first++ = &param_t1;
            *first++ = &param_Us;
            *first++ = nullptr;
        }
    }

    namespace _2a
    {
        static Parameter param_t1("t1", "t1", gset.signals[TypeSignal::_2a].values[0]);

        static Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_2a].values[1]);

        void SetParameters(Item **first)
        {
            *first++ = &param_t1;
            *first++ = &param_Us;
            *first++ = nullptr;
        }
    }

    namespace _2b
    {
        static Parameter param_td("td", "td", gset.signals[TypeSignal::_2b].values[0]);

        static Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_2b].values[1]);

        void SetParameters(Item **first)
        {
            *first++ = &param_td;
            *first++ = &param_Us;
            *first++ = nullptr;
        }
    }

    namespace _3a
    {
        void SetParameters(Item **first)
        {
            *first++ = nullptr;
        }
    }

    namespace _3b
    {
        void SetParameters(Item **first)
        {
            *first++ = nullptr;
        }
    }

    namespace _4
    {
        void SetParameters(Item **first)
        {
            *first++ = nullptr;
        }
    }

    namespace _5a
    {
        void SetParameters(Item **first)
        {
            *first++ = nullptr;
        }
    }

    namespace _5b
    {
        void SetParameters(Item **first)
        {
            *first++ = nullptr;
        }
    }
}
