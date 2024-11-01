#ifndef __UART_DRV_H
#define __UART_DRV_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "drivers_config.h"
#include "string.h"

/* Exported functions --------------------------------------------------------*/
void DRV_UART_IRQHandler(UART_HandleTypeDef *huart);

void USART1_Init(void);
void USART2_Init(void);
void USART3_Init(void);

#endif
