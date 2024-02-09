// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


#define _HAL_GPIO_Init(x, y)    _


struct Settings;


#define TIME_MS   HAL_TIM::TimeMS()


namespace HAL
{
    void Init();
    void DeInit();
    void ERROR_HANDLER();
};


namespace HAL_TIM
{
    void Init();

    uint TimeMS();

    void DelayMS(uint timeMS);

    void DelayUS(uint timeUS);
};


namespace HAL_EEPROM
{
    const uint ADDR_SECTOR_0 = ((uint)0x08000000);  // 16k  Основная прошивка
    const uint ADDR_SECTOR_1 = ((uint)0x08004000);  // 16k
    const uint ADDR_SECTOR_2 = ((uint)0x08008000);  // 16k
    const uint ADDR_SECTOR_3 = ((uint)0x0800c000);  // 16k
    const uint ADDR_SECTOR_4 = ((uint)0x08010000);  // 64k
    const uint ADDR_SECTOR_FIRMWARE = ((uint)0x08020000);  // 128k  Сюда записываем прошивку
    const uint ADDR_SECTOR_UPGRADE = ((uint)0x08040000);  // 128k  Здесь хранится считанная из флешки прошивка
    const uint ADDR_SECTOR_SETTINGS = ((uint)0x08060000);  // 128k
    const uint SIZE_SECTOR_SETTINGS = (128 * 1024);

    void LoadSettings(Settings *settings);

    void SaveSettings(Settings *settings);

    // Стирает сектор с начальным адресом startAddress
    void EraseSector(uint address);

    // Записывает size байт из массива data по адресу address
    void WriteData(uint address, void *data, uint size);
};


namespace HAL_BUS_DISPLAY
{
    void Init();
    void Reset();
    void WriteCommand(uint16 command);
    void WriteCommand(uint16 command, uint data);
    void WriteCommand(uint16 command, uint data1, uint data2, uint data3);
    void WriteData(uint data);
    uint16 ReadData();

    void SendBuffer(uint8 *buffer, int x, int y, int width, int height, int k);

    uint16 GetData(uint16 address);
};


// Используется для связи с основной платой
namespace HAL_SPI1
{
    void Init();

    void Transmit(const void *buffer, int size);

    void Transmit(int value);

    void Receive(void *recv, int size);
}

