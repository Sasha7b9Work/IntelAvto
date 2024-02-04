// 2024/02/01 16:42:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"
#include <stm32f4xx_hal.h>


Pin pin_SPI_SCK(PinMode::SPI_SCK, GPIOA, GPIO_PIN_5);
Pin pin_SPI_MO(PinMode::SPI_MO, GPIOA, GPIO_PIN_7);
Pin pin_SPI_MI(PinMode::SPI_MI, GPIOA, GPIO_PIN_6);
Pin pin_SPI_CS(PinMode::SPI_CS, GPIOA, GPIO_PIN_2);


PinOut pin_A0_RG(GPIOG, GPIO_PIN_5);
PinOut pin_A1_RG(GPIOG, GPIO_PIN_6);
PinOut pin_A2_RG(GPIOG, GPIO_PIN_7);
PinOut pin_A3_RG(GPIOG, GPIO_PIN_8);
PinOut pin_CLK_RG(GPIOD, GPIO_PIN_12);
PinOut pin_DAT_RG(GPIOD, GPIO_PIN_13);
PinOut pin_WR_RG(GPIOD, GPIO_PIN_11);
PinOut pin_ON_OFF(GPIOD, GPIO_PIN_9);
PinOut pin_START(GPIOD, GPIO_PIN_8);
PinOut pin_STOP(GPIOB, GPIO_PIN_15);
PinOut pin_A12(GPIOE, GPIO_PIN_13);
PinOut pin_2A(GPIOE, GPIO_PIN_12);
PinOut pin_3AB(GPIOE, GPIO_PIN_11);
PinOut pin_K1_COM(GPIOC, GPIO_PIN_3);
PinOut pin_K5_COM(GPIOC, GPIO_PIN_2);
PinOut pin_K6_COM(GPIOC, GPIO_PIN_1);
PinOut pin_K7_COM(GPIOC, GPIO_PIN_0);
PinOut pin_K3_COM(GPIOF, GPIO_PIN_10);

PinOut pin_K1_FOR(GPIOF, GPIO_PIN_1);
PinOut pin_K2_FOR(GPIOF, GPIO_PIN_0);
PinOut pin_K_R1(GPIOF, GPIO_PIN_8);

PinOut pin_SCLK_R(GPIOA, GPIO_PIN_12);
PinOut pin_DAT_DAC_R(GPIOA, GPIO_PIN_11);
PinOut pin_CS1_R(GPIOA, GPIO_PIN_10);
PinOut pin_CS2_R(GPIOA, GPIO_PIN_9);
PinOut pin_END_R(GPIOD, GPIO_PIN_1);

PinOut pin_NPULES0(GPIOG, GPIO_PIN_0);
PinOut pin_NPULSE1(GPIOF, GPIO_PIN_15);
PinOut pin_NPULSE2(GPIOF, GPIO_PIN_14);


void HAL_PINS::Init()
{
    pin_SPI_SCK.Init();
    pin_SPI_MO.Init();
    pin_SPI_MI.Init();
    pin_SPI_CS.Init();

    pin_A0_RG.Init();
    pin_A1_RG.Init();
    pin_A2_RG.Init();
    pin_CLK_RG.Init();
    pin_CLK_RG.ToLow();
    pin_DAT_RG.Init();
    pin_WR_RG.Init();
    pin_WR_RG.ToLow();
    pin_ON_OFF.Init();
    pin_ON_OFF.ToLow();
    pin_START.Init();
    pin_START.ToLow();
    pin_STOP.Init();
    pin_STOP.ToLow();

    pin_A12.Init();
    pin_A12.ToLow();
    pin_2A.Init();
    pin_2A.ToLow();
    pin_3AB.Init();
    pin_3AB.ToLow();
    pin_K1_COM.Init();
    pin_K1_COM.ToLow();
    pin_K3_COM.Init();
    pin_K3_COM.ToLow();
    pin_K5_COM.Init();
    pin_K5_COM.ToLow();
    pin_K6_COM.Init();
    pin_K6_COM.ToLow();
    pin_K7_COM.Init();
    pin_K7_COM.ToLow();

    pin_K1_FOR.Init();
    pin_K1_FOR.ToLow();
    pin_K2_FOR.Init();
    pin_K2_FOR.ToLow();
    pin_K_R1.Init();

    pin_SCLK_R.Init();
    pin_DAT_DAC_R.Init();
    pin_CS1_R.Init();
    pin_CS2_R.Init();
    pin_CS2_R.ToHi();
    pin_END_R.Init();
    pin_NPULES0.Init();
    pin_NPULSE1.Init();
    pin_NPULSE2.Init();
}


void PinOut::ToHi()
{
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)port;

    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
}


void PinOut::ToHi(uint timeUS)
{
    ToHi();

    TimeMeterUS().WaitFor(timeUS);

    ToLow();
}


void PinOut::ToLow()
{
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)port;

    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
}


void Pin::Init()
{

}
