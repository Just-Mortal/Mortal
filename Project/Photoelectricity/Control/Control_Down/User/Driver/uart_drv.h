#ifndef __UART_DRV_H
#define __UART_DRV_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include "string.h"

/* Exported functions --------------------------------------------------------*/
void DRV_UART_IRQHandler(UART_HandleTypeDef *huart);

void USART2_Init(void);

#endif
