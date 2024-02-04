// 2024/02/01 16:41:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct PinMode
{
    enum E
    {
        OUTPUT,
        SPI_SCK,
        SPI_MO,
        SPI_MI,
        SPI_CS,
        Count
    };
};


struct Pin
{
    Pin(PinMode::E _mode, void *_port, uint16 _pin) : port(_port), pin(_pin), mode(_mode) { }
    void Init();
protected:
    void      *port;
    uint16     pin;
    PinMode::E mode;
};


struct PinOut : public Pin
{
    PinOut(void *_port, uint16 _pin) : Pin(PinMode::OUTPUT, _port, _pin) { }
    void ToHi();
    void ToHi(uint timeUS);
    void ToLow();
};


struct PinIn : public Pin
{

};


namespace HAL_PINS
{
    void Init();
}


extern Pin pin_SPI_SCK;
extern Pin pin_SPI_MO;
extern Pin pin_SPI_MI;
extern Pin pin_SPI_CS;

extern PinOut pin_A0_RG;    // Выбор адреса внутреннего регистра ПЛИС
extern PinOut pin_A1_RG;    // Выбор адреса внутреннего регистра ПЛИС
extern PinOut pin_A2_RG;    // Выбор адреса внутреннего регистра ПЛИС
extern PinOut pin_A3_RG;    // Выбор адреса внутреннего регистра ПЛИС

extern PinOut pin_CLK_RG;   // Такты запси данных в сдвиговый регистр ПЛИС
extern PinOut pin_DAT_RG;   // Данные для записи данных в сдвиговый регистр ПЛИС
extern PinOut pin_WR_RG;    // Сигнал записи данных из сдвигового регистра в адресуемый региср

extern PinOut pin_ON_OFF;   // Включение/выключение тактирования, лог "1". На время тестирования
                            // этот бит должен быть установлен в состояние лог. "1". Устанавливается
                            // в состояние лог. "0" : при изменении сигнала, при получении
                            // сигнала аварийного выключения

extern PinOut pin_START;    // Сигнал, по которому ПЛИС должна начать формирование тестовых импульсов,
                            // формируется контроллером после выполнения всех подготовительных операций

extern PinOut pin_STOP;     // Сигнал остановки теста, вырабатывается контроллером, по этому сигналу
                            // ПЛИС должна прекратить формирование импульсов

extern PinOut pin_A12;      // Устанавливаются только при выключении источника высокого напряжения
extern PinOut pin_2A;       // +--------+---+---+---+---+---+---+---+---+
extern PinOut pin_3AB;      // | Сигнал |A12|2A |3AB|K1 |K3 |K5 |K6 |K7 |
extern PinOut pin_K1_COM;   // +--------+---+---+---+---+---+---+---+---+
extern PinOut pin_K3_COM;   // | OFF    |   |   |   |   |   |   |   |   |
extern PinOut pin_K5_COM;   // | 1      | 1 |   |   | 1 |   | 1 | 1 | 1 |
extern PinOut pin_K6_COM;   // | 2a     |   | 1 |   | 1 | 1 | 1 |   | 1 |
extern PinOut pin_K7_COM;   // | 3a,b   |   |   | 1 | 1 | 1 |   | 1 |   |
                            // +--------+---+---+---+---+---+---+---+---+

extern PinOut pin_K1_FOR;   // Сигнал включения реле опорного напряжения платы формирователя, подаётся
                            // на ПЛИС, которая формирует сигнал включения/выключения опорного напряжения
                            // платы формирователя
extern PinOut pin_K2_FOR;   // Сигнал включения реле питания истокового повторителя, подаётся на ПЛИС,
                            // которая формирует сигнал включения/выключения питания истокового повторителя

extern PinOut pin_K_R1;     // Сигнал включения реле нагрузки для сигнала 1:
                            // "1" - 12В - 10 Ом
                            // "2" - 24В - 50 Ом

extern PinOut pin_SCLK_R;   // Такты последовательного интерефейса для записи данных по последовательному интерфейсу
                            // в DAC платы источника напряжения

extern PinOut pin_DAT_DAC_R;    // Данные последовательного интерфейса для записи данных по последовательному
                                // интерфейсу в DAC платы источника напряжения

extern PinOut pin_CS1_R;    // Сигнал выбора DAC напряжения питания ИП платы истончика напряжения, активный "0".

extern PinOut pin_CS2_R;    // Сигнал выбора DAC напряжения высоковольтного модуля платы источника напряжения,
                            // активный лог "1"

extern PinOut pin_END_R;    // Сигнал завершения теста, вырабатывается ПЛИС по завершению теста

extern PinOut pin_ENB_HB;   // Управляющий бит модуля питания HVMP 1K-8K 40W. Должен быть установлен в "0" перед 
                            // установкой K1_FOR, K2_FOR

extern PinOut pin_NPULES0;  // Тип формируемого сигнала для ПЛИС
extern PinOut pin_NPULSE1;  // +--------+------+------+------+
extern PinOut pin_NPULSE2;  // | Сигнал |PULSE0|PULSE1|PULSE2|
                            // +--------+------+------+------+
                            // | OFF    |      |      |      |
                            // | 1      |  1   |      |      |
                            // | 2a     |      |  1   |      |
                            // | 3a,b   |  1   |  1   |      |
                            // | OFF    |      |      |  1   |
                            // +--------+------+------+------+
