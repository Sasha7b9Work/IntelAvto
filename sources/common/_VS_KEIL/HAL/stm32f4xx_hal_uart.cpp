#include <stm32f4xx_hal_uart.h>



HAL_StatusTypeDef HAL_UART_Receive_IT(UART_HandleTypeDef *, uint8_t *, uint16_t)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_UART_Transmit(UART_HandleTypeDef *, uint8_t *, uint16_t, uint32_t)
{
    return HAL_OK;
}


HAL_StatusTypeDef HAL_UART_Init(UART_HandleTypeDef *)
{
    return HAL_OK;
}


void HAL_UART_IRQHandler(UART_HandleTypeDef * /*huart*/)
{

}