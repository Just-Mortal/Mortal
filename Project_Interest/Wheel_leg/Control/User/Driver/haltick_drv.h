#ifndef __HALTICK_DRV_H
#define __HALTICK_DRV_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

// Hal库全局时间变量(默认1ms更新一次)
extern __IO uint32_t 				uwTick;
extern HAL_TickFreqTypeDef 	uwTickFreq;
/* Exported macro ------------------------------------------------------------*/
#define millis()    (uwTick)

/* Exported types ------------------------------------------------------------*/
extern volatile uint32_t HalTick;

/* Exported functions --------------------------------------------------------*/
uint32_t micros(void);
void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif
