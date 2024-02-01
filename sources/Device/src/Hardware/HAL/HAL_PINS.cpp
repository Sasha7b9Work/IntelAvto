// 2024/02/01 16:42:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL_PINS.h"
#include <stm32f4xx_hal.h>


Pin pin_SPI_SCK(PinMode::SPI_SCK, GPIOA, GPIO_PIN_5);
Pin pin_SPI_MO(PinMode::SPI_MO, GPIOA, GPIO_PIN_7);
Pin pin_SPI_MI(PinMode::SPI_MI, GPIOA, GPIO_PIN_6);
Pin pin_SPI_CS(PinMode::SPI_CS, GPIOA, GPIO_PIN_2);


PinOut pin_A0_RG(GPIOG, GPIO_PIN_5);
PinOut pin_A1_RG(GPIOG, GPIO_PIN_6);
PinOut pin_A2_RG(GPIOG, GPIO_PIN_7);
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


void PinOut::ToHi()
{
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)port;

    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
}


void PinOut::ToLow()
{
    GPIO_TypeDef *gpio = (GPIO_TypeDef *)port;

    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
}
