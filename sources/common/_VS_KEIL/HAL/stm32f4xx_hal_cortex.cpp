#include <stm32f4xx_hal_conf.h>
#include <stm32f4xx_hal_cortex.h>


extern "C"
{
    void HAL_NVIC_DisableIRQ(IRQn_Type)
    {

    }

    
    uint32_t HAL_SYSTICK_Config(uint32_t /*TicksNumb*/)
    {
        return 0;
    }

    
    void HAL_NVIC_EnableIRQ(IRQn_Type /*IRQn*/)
    {

    }

    
    void HAL_NVIC_SetPriority(IRQn_Type /*IRQn*/, uint32_t /*PreemptPriority*/, uint32_t /*SubPriority*/)
    {

    }
}


void HAL_NVIC_SetPriorityGrouping(uint32_t /*PriorityGroup*/)
{

}

void HAL_SYSTICK_CLKSourceConfig(uint32_t /*CLKSource*/)
{

}
