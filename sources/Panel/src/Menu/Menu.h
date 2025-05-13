// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Primitives_.h"
#include "Hardware/Keyboard/Keyboard_.h"


class Page;


namespace Menu
{
    void Init();

    void Draw();

    Page* OpenedPage();

    void SetOpenedPage(Page *page);

    // ���� ������ ������� �������� �������, �� true
    bool OpenedPageIsSignal();

    namespace Input
    {
        // ��� ������� ���������� ������� �����
        void Update();

        // ��������� ������� ��������� �����
        void SetFuncUpdate(void (*funcUpdate)());

        // ��� ������� ��������� ��������� �� ���������
        void FuncUpdate();

        void FuncEmptyUpdate();

        // ��������� ������� ������
        void OnKey(Key::E);
    };

    extern Primitives::Label labelMode;
};
