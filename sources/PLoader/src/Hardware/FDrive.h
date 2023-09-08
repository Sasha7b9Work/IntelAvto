// 2022/1/16 17:01:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <ff.h>

#define TIME_WAIT   3000    // ����� ������ ��������


struct State
{
    enum E
    {
        Start,
        Mount,            // ������������ ������
        WrongFlash,       // ������ ����, �� ��������� ������
        RequestAction,    // ��� ������ - ���������� ��� ���
        NotFile,          // ���� ���� �������������, �� ���������� �� �� ����
        Upgrade,          // ������� ��������
        Ok                // ���������� ������ ���������
    };
};


class FDrive
{
    friend class CPU;

public:
    static void Init();

    // ������� ������� ����������
    static void AttemptUpdate();

    static bool FileExist(pchar fileName);

    // ��������� ���� ��� ������. ���������� ������ �����. -1 � ������ ��������
    static int OpenForRead(FIL *file, pchar fileName);

    // ��������� �� ��������� ����� numBytes ����. ���������� ����� ������� ��������� ����
    static int Read(FIL *file, int numBytes, void *buffer);
    static int ReadFromPosition(FIL *file, int pos, int numBytes, void *buffer);

    static void Close(FIL *file);

    static void *GetHandleHCD();

    static void *GetHandleUSBH();

    static float PercentsUpdated();

    static State::E State();

    class LL_
    {
    public:
        static void InitHCD(void *phost);

        static void SetToggle(uint8 pipe, uint8 toggle);

        static uint8 GetToggle(uint8 pipe);
    };

private:

    // ������� ���������
    static void EraseSettings();

    // �������� � EEPROM ���� � ��������� � ������
    static bool Upgrade();

    static bool ReadZone(FIL *f_hash, FIL *f_firm, int num_zone, int size_zone, uint8 buffer[1024]);

    static void ReadZones(FIL *f_hash, FIL *f_firm, uint address, const int size);

    static bool ReadSize(FIL *f_hash, FIL *f_firm, int *size);

    static uint CalculateCRC32(char *buffer, int size);
};
