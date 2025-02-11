// 2025/02/11 13:45:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Sound.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"
#include "Utils/Math_.h"
#include <math.h>
#include <stm32f4xx_hal.h>


namespace Sound
{
    static const int POINTS_IN_PERIOD_SOUND = 10;

    static uint8 points[POINTS_IN_PERIOD_SOUND] = { 0 };

    static float frequency = 0.0f;

    static float amplitude = 0.0f;

    static TypeWave::E typeWave = TypeWave::Sine;

    static bool soundWarnIsBeep = false;

    static bool buttonIsPressed = false;    // \brief Когда запускается звук нажатой кнопки, устанавливается этот флаг, чтобы знать, проигрывать ли знак 
                                            // отпускания
    static volatile bool isBeep = false;

    static DAC_HandleTypeDef handleDAC = { DAC };

    static uint16 CalculatePeriodForTIM();

    static void CalculateSine();

    static void CalculateMeandr();

    static void CalculateTriangle();

    static void SetWave();

    static void Sound_Beep(const TypeWave::E newTypeWave, const float newFreq, const float newAmpl, const int newDuration);

    static void TIM7_Config(uint16 prescaler, uint16 period);
}


void Sound::Init()
{
    __DMA1_CLK_ENABLE();        // Для DAC1 (бикалка)
    __TIM7_CLK_ENABLE();        // Для DAC1 (бикалка)


    DAC_ChannelConfTypeDef config =
    {
        DAC_TRIGGER_T7_TRGO,
        DAC_OUTPUTBUFFER_ENABLE
    };

    HAL_DAC_DeInit(&handleDAC);

    HAL_DAC_Init(&handleDAC);

    HAL_DAC_ConfigChannel(&handleDAC, &config, DAC_CHANNEL_1);
}


void Sound::Stop()
{
    HAL_DAC_Stop_DMA(&handleDAC, DAC_CHANNEL_1);
    isBeep = false;
    soundWarnIsBeep = false;
}


void Sound::TIM7_Config(uint16 prescaler, uint16 period)
{
    static TIM_HandleTypeDef htim =
    {
        TIM7, {}, HAL_TIM_ACTIVE_CHANNEL_1, {}, HAL_UNLOCKED, HAL_TIM_STATE_RESET
    };

    htim.Init.Prescaler = prescaler;
    htim.Init.Period = period;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&htim);

    TIM_MasterConfigTypeDef masterConfig =
    {
        TIM_TRGO_UPDATE,
        TIM_MASTERSLAVEMODE_DISABLE
    };

    HAL_TIMEx_MasterConfigSynchronization(&htim, &masterConfig);

    HAL_TIM_Base_Stop(&htim);
    HAL_TIM_Base_Start(&htim);
}


uint16 Sound::CalculatePeriodForTIM()
{
#define MULTIPLIER_CALCPERFORTIM 30e6f

    return (uint16)(MULTIPLIER_CALCPERFORTIM / frequency / POINTS_IN_PERIOD_SOUND);
}


void Sound::CalculateSine()
{
    for(int i = 0; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        float step = 2.0f * 3.1415926f / (POINTS_IN_PERIOD_SOUND - 1);
        float value = (sinf((float)((float)i * step)) + 1.0f) / 2.0f;
        float v = value * amplitude * 255.0f;
        points[i] = (uint8)v;
    }

    for (int i = 0; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        if (i < POINTS_IN_PERIOD_SOUND / 2)
        {
            points[i] = 0;
        }
        else
        {
            points[i] = (uint8)(255.0f * amplitude);
        }
    }
}


void Sound::CalculateMeandr()
{
    for(int i = 0; i < POINTS_IN_PERIOD_SOUND / 2; i++)
    {
        points[i] = (uint8)(255.0f * amplitude);
    }
    for(int i = POINTS_IN_PERIOD_SOUND / 2; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        points[i] = 0;
    }
}


void Sound::CalculateTriangle()
{
    float k = 255.0 / POINTS_IN_PERIOD_SOUND;
    for(int i = 0; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        points[i] = (uint8)(k * (float)i * amplitude);
    }
}



void Sound::SetWave()
{
    TIM7_Config(0, CalculatePeriodForTIM());

    if(typeWave == TypeWave::Sine)
    {
        CalculateSine();
    }
    else if(typeWave == TypeWave::Meandr)
    {
        CalculateMeandr();
    }
    else if(typeWave == TypeWave::Triangle)
    {
        CalculateTriangle();
    }
}


void Sound::Sound_Beep(const TypeWave::E newTypeWave, const float newFreq, const float newAmpl, const int newDuration)
{
    if (soundWarnIsBeep)
    {
        return;
    }

    if (frequency != newFreq || amplitude != newAmpl || typeWave != newTypeWave)
    {
        frequency = newFreq;
        amplitude = newAmpl * 100.0f / 100.0f;
        typeWave = newTypeWave;
        
        Stop();
        SetWave();
    }

    Stop();
    
    isBeep = true;
    
    HAL_DAC_Start_DMA(&handleDAC, DAC_CHANNEL_1, (uint *)points, POINTS_IN_PERIOD_SOUND, DAC_ALIGN_8B_R);

    Timer::SetOnceTask(TimerTask::kStopSound, (uint)newDuration, Stop);
}


void Sound::WaitForCompletion()
{
    while (isBeep)
    {
    }
}


void Sound::ButtonPress()
{
    Sound_Beep(TypeWave::Sine, 2000.0f, 0.75f, 50);
    buttonIsPressed = true;
}


void Sound::ButtonRelease()
{
    if (buttonIsPressed)
    {
        Sound_Beep(TypeWave::Sine, 1000.0f, 0.5f, 50);
        buttonIsPressed = false;
    }
}


void Sound::GovernorChangedValue()
{
    Sound_Beep(TypeWave::Sine, 1000.0f, 0.5f, 50);
    buttonIsPressed = false;
}


void Sound::RegulatorShiftRotate()
{
    Sound_Beep(TypeWave::Sine, 1.0f, 0.2f, 3);
    buttonIsPressed = false;
}


void Sound::RegulatorSwitchRotate()
{
    Sound_Beep(TypeWave::Sine, 500.0f, 0.5f, 75);
    buttonIsPressed = false;
}


void Sound::WarnBeepBad()
{
    Sound_Beep(TypeWave::Meandr, 500.0f, 1.0f, 500);
    soundWarnIsBeep = true;
    buttonIsPressed = false;
}


void Sound::WarnBeepGood()
{
    Sound_Beep(TypeWave::Triangle, 1000.0f, 1.0f, 500);
    buttonIsPressed = false;
}
