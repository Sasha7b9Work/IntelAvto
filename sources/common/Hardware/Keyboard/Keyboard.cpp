// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include <stm32f4xx_hal.h>


namespace Keyboard
{
#define TIME_UPDATE_KEYBOARD 2   // Время между опросами клавиатуры
#define NUM_RL 4
#define NUM_SL 4

#define PIN_SL0  GPIO_PIN_10
#define PORT_SL0 GPIOA

#define PIN_SL1  GPIO_PIN_8
#define PORT_SL1 GPIOA

#define PIN_SL2  GPIO_PIN_9
#define PORT_SL2 GPIOC

#define PIN_SL3  GPIO_PIN_8
#define PORT_SL3 GPIOC

#define PIN_RL0  GPIO_PIN_7
#define PORT_RL0 GPIOC

#define PIN_RL1  GPIO_PIN_6
#define PORT_RL1 GPIOC

#define PIN_RL2  GPIO_PIN_15
#define PORT_RL2 GPIOD

#define PIN_RL3  GPIO_PIN_14
#define PORT_RL3 GPIOD

#define PIN_ENC1  GPIO_PIN_11
#define PORT_ENC1 GPIOD

#define PIN_ENC2  GPIO_PIN_12
#define PORT_ENC2 GPIOD

#define PIN_ENCBUT GPIO_PIN_13
#define PORT_ENCBUT GPIOD


    static TIM_HandleTypeDef handleTIM4;


    static Key::E keys[NUM_SL][NUM_RL] =
    {
        {Key::Left, Key::Right, Key::Back, Key::OK},
        {Key::_1,   Key::_2,    Key::_3,   Key::_4},
        {Key::_5,   Key::_6,    Key::_7,   Key::_8},
        {Key::_9,   Key::Minus, Key::Dot,  Key::Start},
    };

    /// Очередь сообщений - здесь все события органов управления
#define MAX_ACTIONS 100
    static Control controls[MAX_ACTIONS];
    /// Количество уже имеющихся сообщений
    static int numActions = 0;

    /// Установленное в true значение означает, что сохранять куда-либо информацию о её состоянии нельзя до отпускания (чтобы не было ложных срабатываний типа Long
    static bool alreadyLong[NUM_RL][NUM_SL];
    /// При обнаружении нажатия кнопки сюда записывается время нажатия
    static uint timePress[NUM_SL][NUM_RL];

    /// Время последнего автонажатия нопки
    volatile uint prevRepeat = 0;
    volatile uint prevPause = 0;

    /// Установить состояние пина
    static void Set_SL(int, int);
    /// Установить состояние всех пинов в одно положение
    static void Set_All_SL(int);
    /// Возвращает состояние пина rl
    static int Read_RL(int rl);
    /// Инициализировать пины
    static void InitPins();
    /// Инициализировать таймер для периодического опроса клавиатуры
    static void InitTimer();
    /// Функция, периодически вызываемая по прерыванию таймера
    static void Update();
    /// Добавить действие в буфер
    static void AddAction(Key::E key, Action::E action);
    /// Обработка ручки
    static void DetectRegulator();

    static bool KeyboardCheck();

#define BUTTON_IS_PRESS(state) ((state) == 0)
}


void Keyboard::Update()
{
    uint time = HAL_GetTick();

    Set_All_SL(1);

    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        Set_SL(sl, 0);

        for (int rl = 0; rl < NUM_RL; ++rl)
        {
            int state = Read_RL(rl);

            Key::E key = keys[sl][rl];

            if (key != Key::None)
            {
                if (timePress[sl][rl])                      // Если клавиша находится в нажатом положении
                {
                    if (time - timePress[sl][rl] > 100)     // Если прошло более 100 мс с момента нажатия
                    {
                        if (!BUTTON_IS_PRESS(state))        // Если сейчас кнопка находится в отжатом состояини
                        {
                            timePress[sl][rl] = 0;
                            if (!alreadyLong[sl][rl])
                            {
                                AddAction(key, Action::Release);
                            }
                            alreadyLong[sl][rl] = false;
                            prevRepeat = 0;
                        }
                        else if (time - timePress[sl][rl] > 500 && !alreadyLong[sl][rl])
                        {
                            AddAction(key, Action::Long);
                            alreadyLong[sl][rl] = true;
                        }
                    }
                }
                else if (BUTTON_IS_PRESS(state) && !alreadyLong[sl][rl])
                {
                    timePress[sl][rl] = time;
                    AddAction(key, Action::Press);
                    prevRepeat = 0;
                }
            }
        }

        Set_All_SL(1);
    }

    DetectRegulator();
    Set_All_SL(1);
}

void Keyboard::DetectRegulator()
{
    // Детектируем поворот
    static bool prevStatesIsOne = false;

    bool stateLeft = (HAL_GPIO_ReadPin(PORT_ENC1, PIN_ENC1) == GPIO_PIN_SET);
    bool stateRight = (HAL_GPIO_ReadPin(PORT_ENC2, PIN_ENC2) == GPIO_PIN_SET);

    if (stateLeft && stateRight)
    {
        prevStatesIsOne = true;
    }
    else if (prevStatesIsOne && stateLeft && !stateRight)
    {
        prevStatesIsOne = false;

        AddAction(Key::GovLeft, Action::Press);
    }
    else if (prevStatesIsOne && !stateLeft && stateRight)
    {
        prevStatesIsOne = false;

        AddAction(Key::GovRight, Action::Press);
    }
}

bool Keyboard::KeyboardCheck()
{
    bool keyboardFail = false;

    Set_All_SL(1);

    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        Set_SL(sl, 0);

        for (int rl = 0; rl < NUM_RL; ++rl)
        {
            int state = Read_RL(rl);

            Key::E key = keys[sl][rl];

            if (key != Key::None)
            {
                if (BUTTON_IS_PRESS(state))
                {
                    keyboardFail = true;
                }
            }
        }

        Set_All_SL(1);
    }
    return keyboardFail;
}

bool Keyboard::Init()
{
    for (int sl = 0; sl < NUM_SL; ++sl)
    {
        for (int rl = 0; rl < NUM_RL; ++rl)
        {
            timePress[sl][rl] = 0;
        }
    }

    InitPins();
    InitTimer();
    return KeyboardCheck();
}


void Keyboard::Set_All_SL(int st)
{
    for (int i = 0; i < 4; i++)
    {
        Set_SL(i, st);
    }
}


void Keyboard::Set_SL(int bus, int st)
{
    static const GPIO_TypeDef *ports[4] = { PORT_SL0, PORT_SL1, PORT_SL2, PORT_SL3 };
    static const uint16 pins[4] = { PIN_SL0,  PIN_SL1,  PIN_SL2,  PIN_SL3 };
    static const GPIO_PinState state[2] = { GPIO_PIN_RESET, GPIO_PIN_SET };

    HAL_GPIO_WritePin((GPIO_TypeDef *)ports[bus], pins[bus], state[st]); //-V2567
}


int Keyboard::Read_RL(int rl)
{
    static const GPIO_TypeDef *ports[4] = { PORT_RL0, PORT_RL1, PORT_RL2, PORT_RL3 };
    static const uint16 pins[4] = { PIN_RL0,  PIN_RL1,  PIN_RL2, PIN_RL3 };

    return HAL_GPIO_ReadPin((GPIO_TypeDef *)ports[rl], pins[rl]); //-V2567
}


void Keyboard::InitPins()
{
    GPIO_InitTypeDef is =
    {
        PIN_SL0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLUP
    };

    HAL_GPIO_Init(PORT_SL0, &is);

    is.Pin = PIN_SL1;
    HAL_GPIO_Init(PORT_SL1, &is);

    is.Pin = PIN_SL2;
    HAL_GPIO_Init(PORT_SL2, &is);

    is.Pin = PIN_SL3;
    HAL_GPIO_Init(PORT_SL3, &is);

    is.Mode = GPIO_MODE_INPUT;

    is.Pin = PIN_RL0;
    HAL_GPIO_Init(PORT_RL0, &is);

    is.Pin = PIN_RL1;
    HAL_GPIO_Init(PORT_RL1, &is);

    is.Pin = PIN_RL2;
    HAL_GPIO_Init(PORT_RL2, &is);

    is.Pin = PIN_RL3;
    HAL_GPIO_Init(PORT_RL3, &is);

    is.Pin = PIN_ENC1;
    HAL_GPIO_Init(PORT_ENC1, &is);

    is.Pin = PIN_ENC2;
    HAL_GPIO_Init(PORT_ENC2, &is);

    is.Pin = PIN_ENCBUT;
    HAL_GPIO_Init(PORT_ENCBUT, &is);
}


void Keyboard::InitTimer()
{
    __HAL_RCC_TIM4_CLK_ENABLE();

    // Инициализируем таймер, по прерываниям которого будем опрашивать клавиатуру
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);

    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    handleTIM4.Instance = TIM4;
    handleTIM4.Init.Period = TIME_UPDATE_KEYBOARD * 10 - 1;
    handleTIM4.Init.Prescaler = (uint)((SystemCoreClock / 2) / 10000) - 1;
    handleTIM4.Init.ClockDivision = 0;
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;

    if (HAL_TIM_Base_Init(&handleTIM4) != HAL_OK)
    {
        HAL::ERROR_HANDLER();
    }

    if (HAL_TIM_Base_Start_IT(&handleTIM4) != HAL_OK)
    {
        HAL::ERROR_HANDLER();
    }

    Set_All_SL(1);
}


void Keyboard::AddAction(Key::E key, Action::E action)
{
    if (action != Action::Press)
    {
        return;
    }

    controls[numActions++] = Control(key, action);
}


bool Keyboard::Empty()
{
    return numActions == 0;
}



Control Keyboard::NextControl()
{
    if (Empty())
    {
        return Control();
    }

    Control result = controls[0];

    for (int i = 1; i < numActions; i++)
    {
        controls[i - 1] = controls[i];
    }

    --numActions;
    return result;
}


String Key::Name(E value)
{
    static pchar names[] =
    {
        "Ручка нажать",
        "РЕЖИМ",
        "ИНДИКАЦИЯ",
        "3",
        "<-",
        "->",
        "6",
        "7",
        "8",
        "9",
        "A",
        "B",
        "C",
        "D",
        "F5",
        "ESC",
        "Left",
        "Right",
        "Ручка влево",
        "Ручка вправо",
        "Channel",
        "ТЕСТ",
        "АВТО",
        "NULL"
    };

    return String(names[value]);
}


#ifdef __cplusplus
extern "C" {
#endif

    void TIM4_IRQHandler();

    void TIM4_IRQHandler()
    {
        HAL_TIM_IRQHandler(&Keyboard::handleTIM4);
    }

#ifdef __cplusplus
}
#endif


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //-V2009 //-V2558
{
    if (htim == &Keyboard::handleTIM4)
    {
        Keyboard::Update();
    }
}


void Keyboard::Lock()
{
    Menu::Input::SetFuncUpdate(Menu::Input::FuncEmptyUpdate);
}


void Keyboard::Unlock()
{
    Menu::Input::SetFuncUpdate(Menu::Input::FuncUpdate);
}
