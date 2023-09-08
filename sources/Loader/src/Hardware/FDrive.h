// 2022/1/16 17:01:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <ff.h>

#define TIME_WAIT   3000    // Время работы заставки


struct State
{
    enum E
    {
        Start,
        Mount,            // Монтирование флешки
        WrongFlash,       // Флешка есть, но прочитать нельзя
        RequestAction,    // Что делать - апгрейдить или нет
        NotFile,          // Если диск примонтирован, но обновления на нём нету
        Upgrade,          // Процесс апгрейда
        Ok                // Обновление удачно завершено
    };
};


class FDrive
{
    friend class CPU;

public:
    static void Init();

    // Сделать попытку обновления
    static void AttemptUpdate();

    static bool FileExist(pchar fileName);

    // Открывает файл для чтения. Возвращает размер файла. -1 в случае неуспеха
    static int OpenForRead(FIL *file, pchar fileName);

    // Считывает из открытого файла numBytes байт. Возвращает число реально считанных байт
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

    // Стереть настройки
    static void EraseSettings();

    // Записать в EEPROM файл с прошивкой с флешки
    static bool Upgrade();

    static bool ReadZone(FIL *f_hash, FIL *f_firm, int num_zone, int size_zone, uint8 buffer[1024]);

    static void ReadZones(FIL *f_hash, FIL *f_firm, uint address, const int size);

    static bool ReadSize(FIL *f_hash, FIL *f_firm, int *size);

    static uint CalculateCRC32(char *buffer, int size);
};
