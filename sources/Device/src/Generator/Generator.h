// 2024/02/01 23:52:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Value;


namespace Generator
{
    void Start1_12V(const Value &Us, const Value &t1);

    void Start1_24V(const Value &Us, const Value &t1);

    void Start2A(const Value &Us, const Value &t1);

    void Start3A(const Value &US);

    void Start3B(const Value &Us);

    void Pause();

    void Resume();

    void Stop();
}
