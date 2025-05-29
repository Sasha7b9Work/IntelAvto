// 2025/05/29 11:47:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <stm32f4xx_hal.h>


class Timer4XX
{
public:
    void Init(TIM_TypeDef *instance, uint prescaler, uint counterMode, uint period, uint clcDiv);

    void EnabledIRQ(uint mainPriority, uint subPriority);

    void DisableIRQ();

    void Start();

    void Stop();

    void StartIT(uint period);

    void StopIT();

    void DeInit();  

    TIM_HandleTypeDef handler;

private:
    IRQn_Type GetIRQn_Type();
};
