// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Log.h"
#include "Settings.h"
#include "Calculate/ValueFPGA.h"
#include "Display/Display.h"
#include "Hardware/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageIndication.h"
#include "Menu/Pages/Modes/Modes.h"
#include "Menu/Pages/Channels/Channels.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdio>
#include <stm32f4xx_hal.h>


#define ReadPin(x)  HAL_GPIO_ReadPin(x)
#define SetPin(x)   HAL_GPIO_WritePin(x, GPIO_PIN_SET)
#define ResetPin(x) HAL_GPIO_WritePin(x, GPIO_PIN_RESET)

#define PinRD       GPIOC, GPIO_PIN_8                   // Флаг готовности чтения
#define PinWR       GPIOC, GPIO_PIN_9                   // Флга готовности к записи
#define PinCS       GPIOB, GPIO_PIN_12
#define PinDATA     GPIOB, GPIO_PIN_15
#define PinCLOCK    GPIOB, GPIO_PIN_13

#define Set_CS      SetPin(PinCS);
#define Reset_CS    ResetPin(PinCS);

#define Set_CLOCK   SetPin(PinCLOCK);   DELAY
#define Reset_CLOCK ResetPin(PinCLOCK); DELAY

#define Set_DATA    SetPin(PinDATA);    DELAY
#define Reset_DATA  ResetPin(PinDATA);  DELAY

#define Flag_RD     ReadPin(PinRD)

#define Read_WR     ReadPin(PinWR)

#define DELAY  HAL_TIM::DelayUS(2)


static uint ident = 0;      // Это значение считывается непосредственно из FPGA
static char encData[10];
static bool autoMode = false;
static bool isOverloaded = false;

uint FPGA::GovernorData::kCalib = 0;     // Это значение считывается непосредственно из FPGA
int FPGA::GovernorData::NAC = 0;         // Поправка для калибровочного коэффициента

uint FPGA::timeChangeSettings = 0;


int    FPGA::Auto::NA = 0;
int    FPGA::Auto::NB = 0;

uint   FPGA::Auto::fpgaMin = 0;
uint   FPGA::Auto::fpgaMid = 0;
uint   FPGA::Auto::fpgaMax = 0;


void FPGA::CycleRead(int numBits, uint &value, bool verifyOnOverload)
{
    value = 0;

    for (int i = numBits - 1; i >= 0; i--)
    {
        Set_CLOCK;
        value |= (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_14) << i);
        Reset_CLOCK;
    }

    if(verifyOnOverload)
    {
        isOverloaded = (value & 1U) != 0;
    };
}


void FPGA::CycleWrite(uint value, int numBits)
{
    for (int i = 0; i < numBits; i++)
    {
        WriteBit(((value) >> i) & 0x1);
    }
}


void FPGA::WriteBit(uint bit)
{
    HAL_GPIO_WritePin(PinDATA, (bit == 0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    DELAY;
    Set_CLOCK;
    Reset_CLOCK;
}


void FPGA::Init()
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    
    GPIO_InitTypeDef is =
    {
        GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };
    HAL_GPIO_Init(GPIOB, &is);

    is.Pin = GPIO_PIN_14;
    is.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOB, &is);

    is.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOC, &is);

    Reset_CS;
}


void FPGA::Update()
{
    if(autoMode)
    {
        ReadAutoMode();
    }
    else
    {
        if (TIME_MS - timeChangeSettings < 200)
        {
            return;
        }

        if (Channel::Current()->mod.typeMeasure.IsDuration() && ModeDuration::Current().IsNdt_1ns())
        {
            ReadInterpolator();
        }
        else if(Channel::Current()->mod.typeMeasure.IsDuration() && 
            (ModeDuration::Current().IsFillFactor() || ModeDuration::Current().IsPhase()))
        {
            ReadFillFactorPhase();
        }
        else if (CURRENT_CHANNEL_IS_A && 
            (Channel::A->mod.modeFrequency.IsComparator() && Channel::A->mod.typeMeasure.IsFrequency())) 
        {
            ReadComparator();
        }
        else
        {
            if (Flag_RD != 0)
            {
                uint counterA = 0;
                uint counterB = 0;

                Set_CS;

                CycleRead(32, counterA, true);
              
                if((ModeFrequency::Current().IsRatioAC() || ModeFrequency::Current().IsRatioBC()) &&
                    Relation::IsEnabled())
                {
                    CycleRead(32, counterB, true);
                }

                Reset_CS;

//                LOG_WRITE("%d %d", counterA, counterB);
                ValueFPGA::Create(counterA, counterB);

                HAL_TIM::DelayUS(8);
            }
        }
    }
} 


void FPGA::ReadFillFactorPhase()
{
    if (Flag_RD != 0)
    {
        uint period = 0;
        uint duration = 0;

        Set_CS;
        CycleRead(32, period, true);
        CycleRead(32, duration, true);
        Reset_CS;

///        LOG_WRITE("%d %d", period, duration);
        ValueFPGA::Create(period, duration);

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::ReadInterpolator()
{
    if (Flag_RD != 0)
    {
        uint timer = 0;
        uint cal1 = 0;
        uint cal2 = 0;

        Set_CS;
        CycleRead(3, ident, false);
        CycleRead(24, timer, false);
        CycleRead(24, cal1, false);
        CycleRead(24, cal2, false);
        Reset_CS;

//        LOG_WRITE("%d %d %d", timer, cal1, cal2);
        ValueFPGA::Create(timer, cal1, cal2);

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::ReadAutoMode()
{
    if (Flag_RD != 0)
    {
        Set_CS;
        CycleRead(3, ident, false);
        CycleRead(10, Auto::fpgaMin, false);
        CycleRead(10, Auto::fpgaMid, false);
        CycleRead(10, Auto::fpgaMax, false);
        Reset_CS;

        Display::Refresh();

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::ReadComparator()
{
    if (Flag_RD != 0)
    {
        uint counter = 0;
        uint interpol1 = 0;
        uint cal1 = 0;
        uint interpol2 = 0;
        uint cal2 = 0;

        Set_CS;
        CycleRead(3, ident, false);
        CycleRead(32, counter, false);
        CycleRead(16, interpol1, false);
        CycleRead(16, cal1, false);
        CycleRead(16, interpol2, false);
        CycleRead(16, cal2, false);
        Reset_CS;

        LOG_WRITE("%d %d %d %d %d", counter, interpol1, cal1, interpol2, cal2);
        ValueFPGA::Create(counter, interpol1, cal1, interpol2, cal2);

        HAL_TIM::DelayUS(8);
    }
}


void FPGA::GovernorData::IncreaseN()
{
    if(PageIndication::calibrationMode.IsEnabled())
    {
        NAC++;
    }
    else
    {
        if(CURRENT_CHANNEL_IS_A)
        {
            Auto::NA++;
        }
        else if(CURRENT_CHANNEL_IS_B)
        {
            Auto::NB++;
        }
    }
}

void FPGA::GovernorData::DecreaseN()
{       
    if(PageIndication::calibrationMode.IsEnabled())
    {
        NAC--;
    }
    else
    {
        if(CURRENT_CHANNEL_IS_A)
        {
            Auto::NA--;
        }
        else if(CURRENT_CHANNEL_IS_B)
        {
            Auto::NB--;
        }
    }
}

void FPGA::GovernorData::SetN(int num)
{       
    if(PageIndication::calibrationMode.IsEnabled())
    {
        NAC = num;
    }
    else
    {
        if(CURRENT_CHANNEL_IS_A)
        {
            Auto::NA = num;
        }
        else if(CURRENT_CHANNEL_IS_B)
        {
            Auto::NB = num;
        }
    }
}


void FPGA::EnableAuto()
{
    autoMode = true;
}


void FPGA::DisableAuto()
{
    autoMode = false;
}


bool FPGA::InAutoMode()
{
    return autoMode;
}


void FPGA::ReadValueCalibrator()
{
    uint calib = 0;

    while (Flag_RD == 0)
    {
    }

    Set_CS;

    CycleRead(3, ident, false);

    CycleRead(10, calib, false);

    Reset_CS;

    HAL_TIM::DelayUS(8);

    GovernorData::SetValueCalibrator(calib);
}


void FPGA::GovernorData::SetValueCalibrator(uint value)
{
    kCalib = value;
    NAC = 0;
}


void FPGA::GovernorData::Write()
{
    Calculate();

    if (Read_WR != 0)           // \todo К сожалению, флаг готовности не работает так, как надо и если ожидать его установки в ноль, то происходит сбой передачи данных
    {                           // Если флаг не готов, выходим. Передавать нужно только если флаг уже установлен в 0
        return;
    }
    
    Reset_CLOCK;
    Set_DATA;
    Set_CLOCK;
    Reset_CLOCK;

    for (int i = 9; i > -1; i--)
    {
        WriteBit((uint)encData[i]);
    }

    Reset_DATA; //-V525
    Set_CLOCK;
    Set_DATA;

    Reset_CLOCK;
    Reset_DATA;

//    while (Read_READY == 0)
//    {
//    }
}


void FPGA::WriteCommand(const Command &command)
{
    while (Read_WR != 0)             // \todo Провеерить. Возможно, по аналогии с передачей данных нельзя ожидать флага готовности
    {
    }

    Reset_CLOCK; //-V525
    Reset_DATA;
    Set_CLOCK;
    Reset_CLOCK;

    CycleWrite(command.value, 10);

    Reset_DATA; //-V525
    Set_CLOCK;
    Set_DATA;

    Reset_CLOCK;
    Reset_DATA;

    SetInvalidData();
}

void FPGA::GovernorData::Reset()
{
    NAC = 0;
}


int FPGA::GovernorData::ValueCalibrator()
{
    int value = (int)kCalib + NAC;

    LIMITATION_BELOW(value, 0);

    return value;
}


void FPGA::GovernorData::Calculate()
{
    if (PageIndication::calibrationMode.IsEnabled())
    {
        DecToBin(ValueCalibrator(), encData);
    }
    else
    {
        int negative = 1024;

        if (CURRENT_CHANNEL_IS_A)
        {
            if (Auto::NA < 0)
            {
                DecToBin(negative + Auto::NA, encData);
            }
            else
            {
                DecToBin(Auto::NA, encData);
            }
        }
        else if (CURRENT_CHANNEL_IS_B)
        {
            if (Auto::NB < 0)
            {
                DecToBin(negative + Auto::NB, encData);
            }
            else
            {
                DecToBin(Auto::NB, encData);
            }
        }
    }
}


bool FPGA::IsOverloaded()
{
    return isOverloaded;
}


void FPGA::SetInvalidData()
{
    timeChangeSettings = TIME_MS;

    ValueFPGA::SetInvalidData();
}


void FPGA::DecToBin(int dec, char *bin)
{
    int x = dec;
    for (int i = 0; i < 10; i++)
    {
        if (x % 2 != 0) { bin[i] = 1; }
        else { bin[i] = 0; }
        x = x / 2;
    }
}


String FPGA::BinToString(pString bin, int num)
{
    char buffer[20];

    for (int i = 0; i < num; i++)
    {
        if (bin[i] == 0)
        {
            buffer[i] = '0';
        }
        else
        {
            buffer[i] = '1';
        }
    }

    buffer[num] = '\0';

    return String(buffer);
}


bool FPGA::Auto::ObtainedResult()
{
    return (fpgaMid != 0) || (fpgaMax != 0) || (fpgaMin != 0);
}


String FPGA::Auto::Give()
{
    if (CURRENT_CHANNEL_IS_A)
    {
        LEVEL_SYNCH_A = ((int)fpgaMid - 512) * 2;
        NA = (int)fpgaMid - 512;
    }

    if (CURRENT_CHANNEL_IS_B)
    {
        LEVEL_SYNCH_B = ((int)fpgaMid - 512) * 2;
        NB = (int)fpgaMid - 512;

    }

    return String("Макс %s Мин %s",
        SU::Int2String(((int)fpgaMax - 512) * 2).c_str(),
        SU::Int2String(((int)fpgaMin - 512) * 2).c_str());
}


void FPGA::Auto::Refresh()
{
    for (int i = 0; i < 10; i++)
    {
        fpgaMin = 0;
        fpgaMid = 0;
        fpgaMax = 0;
    }
}
