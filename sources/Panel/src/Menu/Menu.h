// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Primitives_.h"


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
    };

    void ChangeStateFlashDrive();

    extern Primitives::Label labelMode;
};
