// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings;


#define ERROR_HANDLER() HAL::ErrorHandler()


namespace HAL
{
    void Init();

    void ErrorHandler();
};


// Связь с панелью
namespace HAL_SPI1
{
    void Init();

    void Receive(void *buffer, int size);

    uint ReceiveUInt();

    void Transmit(void *buffer, int size);

    void TransmitUInt(uint);

    // Ожидает начала чипселекта
    void WaitCS();
}


namespace HAL_TIM
{
    void Init();

    uint TimeMS();

    void DelayMS(uint timeMS);

    void DelayUS(uint timeUS);
};


namespace HAL_TIM2
{
    void Init();

    uint GetTicks();

    void StartMultiMeasurement();

    uint TimeUS();
};


namespace HAL_TIM3
{
    void Init();

    void StartIT(uint period);

    void StopIT();
};
