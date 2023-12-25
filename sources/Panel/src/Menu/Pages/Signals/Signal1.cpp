// 2023/12/25 17:20:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Signals/PageSignals.h"



namespace PageSignals
{
    namespace _1
    {
        static Value value_t1;
        static Value value_Us;

        static Parameter param_t1("t1", "t1", value_t1);

        static Parameter param_Us("Us", "Us", value_Us);

        void SetParameters(Item **first)
        {
            *first++ = &param_t1;
            *first++ = &param_Us;
            *first++ = nullptr;
        }
    }

    namespace _2a
    {
        void SetParameters(Item **first)
        {
            *first++ = nullptr;
        }
    }

    namespace _2b
    {
        void SetParameters(Item **first)
        {
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
