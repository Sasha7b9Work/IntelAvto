// 2023/12/25 17:20:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Signals/PageSignals.h"
#include "Settings/Settings.h"


namespace PageSignals
{
    namespace _1
    {
        static Parameter param_Us("Us", "Us", gset.signals[TypeSignal::_1].values[0]);

        static Parameter param_t1("t1", "t1", gset.signals[TypeSignal::_1].values[1]);

        void SetParameters(Item **first)
        {
            *first++ = &param_Us;
            *first++ = &param_t1;
            *first++ = nullptr;
        }
    }

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
