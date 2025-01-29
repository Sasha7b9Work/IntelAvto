// 2024/02/02 00:02:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Value;


struct TypeSignal
{
    enum E
    {
        _1_12V,
        _1_24V,
        _2a,
        _3a,
        _3b,
        Off,
        Count
    };

    static void Set(E v) { current = v; }

    static E Current() { return current; }

    static bool Is1_12V() { return current == _1_12V; }
    static bool Is1_24V() { return current == _1_24V; }
    static bool Is2a() { return current == _2a; }
    static bool Is3a() { return current == _3a; }
    static bool Is3b() { return current == _3b; }
    static bool IsOff() { return current == Off; }

private:

    static E current;
};



namespace FPGA
{
    void Init();

    void Start();

    void Pause();

    void Resume();

    void Stop();

    void SetTypeSignal(TypeSignal::E);

    void WritePeriod(TypeSignal::E, const Value &);
}
