// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


#define _HAL_GPIO_Init HAL_GPIO_Init


struct Settings;


#define TIME_MS   HAL_TIM::TimeMS()


struct HAL
{
    static void Init();
    static void DeInit();
    static void ERROR_HANDLER();
};


struct HAL_EEPROM
{
    static const uint ADDR_SECTOR_0        = ((uint)0x08000000);  // 16k  Основная прошивка
    static const uint ADDR_SECTOR_1        = ((uint)0x08004000);  // 16k
    static const uint ADDR_SECTOR_2        = ((uint)0x08008000);  // 16k
    static const uint ADDR_SECTOR_3        = ((uint)0x0800c000);  // 16k
    static const uint ADDR_SECTOR_4        = ((uint)0x08010000);  // 64k
    static const uint ADDR_SECTOR_FIRMWARE = ((uint)0x08020000);  // 128k  Сюда записываем прошивку
    static const uint ADDR_SECTOR_UPGRADE  = ((uint)0x08040000);  // 128k  Здесь хранится считанная из флешки прошивка
    static const uint ADDR_SECTOR_SETTINGS = ((uint)0x08060000);  // 128k
    static const uint SIZE_SECTOR_SETTINGS = (128 * 1024);

    static void LoadSettings(Settings *settings);

    static void SaveSettings(Settings *settings);

    // Стирает сектор с начальным адресом startAddress
    static void EraseSector(uint address);

    // Записывает size байт из массива data по адресу address
    static void WriteData(uint address, void *data, uint size);
};


struct HAL_FSMC
{
    static void Init();
    static void Reset();
    static void WriteCommand(uint16 command);
    static void WriteCommand(uint16 command, uint data);
    static void WriteCommand(uint16 command, uint data1, uint data2, uint data3);
    static void WriteData(uint data);
    static uint16 ReadData();

    static void SendBuffer(uint8 *buffer, int x, int y, int width, int height, int k);
    
    static uint16 GetData(uint16 address);
};


struct HAL_PCD
{
    // PCD_HandleTypeDef
    static void *handle;
};


struct HAL_TIM
{
    static void Init();

    static uint TimeMS();

    static void DelayMS(uint timeMS);

    static void DelayUS(uint timeUS);
};


struct HAL_USBD
{
    // USBD_HandleTypeDef
    static void *handle;

private:
    friend class VCP;

    static void Init();

    static bool PrevSendingComplete();

    static void SetBufferTX(uint8 *buffer, uint size);

    static void Flush(uint8 *buffer, int size);

    static void SendDataSynch(int sizeBuffer, uint sizeSend, uint8 *buffSend, char *buffer);
};
